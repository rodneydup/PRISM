#include "PluginProcessor.hpp"

PluginProcessor::PluginProcessor()
  : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                     ),
    audioGraph(std::make_unique<juce::AudioProcessorGraph>()) {
  // tell the ProcessorPlayer what audio callback function to play (.get() needed since audioGraph
  // is a unique_ptr)
  processorPlayer.setProcessor(audioGraph.get());

  audioGraph->clear();  // likely not needed but won't hurt

  // Add nodes to the graph
  // Input
  audioInputNode =
    audioGraph->addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
      juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
  // Output
  audioOutputNode =
    audioGraph->addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
      juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));

  // Dilate
  for (int i = 0; i < getBusesLayout().getMainInputChannels(); i++)
    dilateNodes.push_back(audioGraph->addNode(std::make_unique<DilateProcessor>()));

  // Connections
  for (int i = 0; i < getBusesLayout().getMainInputChannels(); i++) {
    audioGraph->addConnection(
      {{audioInputNode->nodeID, i}, {dilateNodes[i]->nodeID, 0}});  // connect input to dilate
    audioGraph->addConnection(
      {{dilateNodes[i]->nodeID, 0}, {audioOutputNode->nodeID, i}});  // connect dilate to output
  }

  // add parameters
  addParameter(fftOrderMenu = new juce::AudioParameterChoice(
                 "fftOrderMenu", "FFT Order", {"128", "256", "512", "1024", "2048", "4096", "8192"},
                 3, "FFT Order"));
  // get names of available windows in juce
  juce::StringArray juceWindows;
  for (int i = 0; i < juce::dsp::WindowingFunction<float>::numWindowingMethods; i++)
    juceWindows.add(juce::dsp::WindowingFunction<float>::getWindowingMethodName(
      juce::dsp::WindowingFunction<float>::WindowingMethod(i)));

  addParameter(focalPointSlider = new juce::AudioParameterFloat(
                 "focalPoint", "Focal Point",
                 juce::NormalisableRange<float>(0.0f, 10000.0f, 0.1, 0.4), 400.0f, "Focal Point",
                 juce::AudioProcessorParameter::genericParameter));

  focalPoint.reset(2048);

  addParameter(dilationFactorSlider = new juce::AudioParameterFloat(
                 "dilationFactor", "Dilation Factor",
                 juce::NormalisableRange<float>(-2.0f, 2.0f, 0.0001), 1.0f));
  dilationFactor.reset(2048);
}

PluginProcessor::~PluginProcessor() {}

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  audioGraph->setPlayConfigDetails(getMainBusNumInputChannels(), getMainBusNumOutputChannels(),
                                   sampleRate, samplesPerBlock);

  audioGraph->prepareToPlay(sampleRate, samplesPerBlock);
}

void PluginProcessor::releaseResources() { audioGraph->releaseResources(); }

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                   juce::MidiBuffer& midiBuffer) {
  for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
    audioBuffer.clear(i, 0, audioBuffer.getNumSamples());

  for (int chan = 0; chan < audioBuffer.getNumChannels(); chan++) {
    if (*fftOrderMenu != dilateNodes[chan]->getProcessor().- 7) changeOrder(*fftOrderMenu + 7);
    float* channelData = audioBuffer.getWritePointer(0, 0);
    for (int i = 0; i < audioBuffer.getNumSamples(); ++i) {
      // iterate smoothvalues per sample
      focalPoint.getNextValue();
      dilationFactor.getNextValue();
    }
  }
  audioGraph->processBlock(audioBuffer, midiBuffer);

  // set new targets for smoothvalues per buffer if slider changed
  if (focalPointSlider->get() != focalPoint.getTargetValue())
    focalPoint.setTargetValue(focalPointSlider->get());
  if (dilationFactorSlider->get() != dilationFactor.getTargetValue())
    dilationFactor.setTargetValue(dilationFactorSlider->get());
}

//==============================================================================

//==============================================================================
// Plug-in stuff

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
  // Only mono/stereo and input/output must have same layout
  const auto& mainOutput = layouts.getMainOutputChannelSet();
  const auto& mainInput = layouts.getMainInputChannelSet();

  // input and output layout must either be the same or the input must be disabled altogether
  if (!mainInput.isDisabled() && mainInput != mainOutput) return false;

  // do not allow disabling the main buses
  if (mainOutput.isDisabled()) return false;

  // only allow stereo and mono
  if (mainOutput.size() > 2) return false;

  return true;
}
const juce::String PluginProcessor::getName() const {
#if JucePlugin_Build_VST3
  return JucePlugin_Name;
#else
  return JUCE_APPLICATION_NAME_STRING;
#endif
}

bool PluginProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool PluginProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool PluginProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const { return 0.0; }

int PluginProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0 programs,
             // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram() { return 0; }

void PluginProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }

const juce::String PluginProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void PluginProcessor::changeProgramName(int index, const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
  juce::ignoreUnused(data, sizeInBytes);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new PluginProcessor(); }
