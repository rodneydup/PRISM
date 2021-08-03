// To Do:
// Range for bin masking
// separate stereo channels
//

#include "MaskProcessor.hpp"

MaskProcessor::MaskProcessor()
  : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ) {
  for (int i = 0; i < getBusesLayout().getMainInputChannels(); i++) {
    IObuffers.push_back(std::make_unique<IObuffer>());
  }
  forwardFFT = std::make_unique<juce::dsp::FFT>(fftOrder);
  inverseFFT = std::make_unique<juce::dsp::FFT>(fftOrder);
  window.resize(fftSize + 1);
  juce::dsp::WindowingFunction<float>::fillWindowingTables(
    window.data(), fftSize + 1, juce::dsp::WindowingFunction<float>::WindowingMethod(windowIndex));
  for (int chan = 0; chan < IObuffers.size(); chan++) {
    IObuffers[chan]->inputBuffer.resize(overlap);
    IObuffers[chan]->inputBufferIndex.resize(overlap);
    for (int i = 0; i < overlap; i++) {
      IObuffers[chan]->inputBuffer[i].resize(fftSize * 2, 0.0f);
      IObuffers[chan]->inputBufferIndex[i] = 0 - hopSize * i;
    }
    IObuffers[chan]->outputQueue.resize(fftSize * 2, 0.0f);

    unmaskedBins[chan].clear();
    maskedBins[chan].clear();

    bins[chan].reserve(4096);
    bins[chan].resize(fftSize / 2, 1);
    for (int i = 0; i < fftSize / 2; i++) unmaskedBins[chan].push_back(bins[chan][i]);
  }

  // add parameters
  addParameter(fftOrderMenu = new juce::AudioParameterChoice(
                 "fftOrderMenu", "FFT Order", {"128", "256", "512", "1024", "2048", "4096", "8192"},
                 fftOrder - 7, ""));
  // get names of available windows in juce
  juce::StringArray juceWindows;
  for (int i = 0; i < juce::dsp::WindowingFunction<float>::numWindowingMethods; i++)
    juceWindows.add(juce::dsp::WindowingFunction<float>::getWindowingMethodName(
      juce::dsp::WindowingFunction<float>::WindowingMethod(i)));

  addParameter(maskedBinsSlider = new juce::AudioParameterFloat(
                 "maskedBins", "Masked Bins", juce::NormalisableRange<float>(0.0, 1.0, 0.0001),
                 1.0f, "", juce::AudioProcessorParameter::genericParameter, stringFromValue,
                 valueFromString));
  addParameter(unmaskedBinsSlider = new juce::AudioParameterFloat(
                 "unmaskedBins", "Unmasked Bins", juce::NormalisableRange<float>(0.0, 1.0, 0.0001),
                 1.0f, "", juce::AudioProcessorParameter::genericParameter, stringFromValue,
                 valueFromString));
  addParameter(maskChangeIntervalSlider =
                 new juce::AudioParameterInt("MaskChangeInterval", "Mask Change Interval", 2, 200,
                                             16, "", stringFromValueInt, valueFromStringInt));

  addParameter(makeUpGain = new juce::AudioParameterBool("makeupgain", "Auto Make-Up Gain", 0));
  addParameter(bypass = new juce::AudioParameterBool("bypass", "Bypass", 0));
  addParameter(oscOn = new juce::AudioParameterBool("oscOn", "OSC On", 0));

  oscAddress = std::make_unique<juce::Label>("OSCAddress", "OSC Address");
  oscAddress->setEditable(true);
  oscAddress->setText("16447", juce::NotificationType::dontSendNotification);

  MakeUpFactor.reset(2048);
  makeUpValues.resize(10, 0.0f);

  srand(std::time(0));

  juce::OSCReceiver::connect(oscAddress->getText().getIntValue());
  juce::OSCReceiver::addListener(this, "/measurement");
}

MaskProcessor::~MaskProcessor() {}

//==============================================================================
void MaskProcessor::prepareToPlay(double, int) {}
void MaskProcessor::releaseResources() {}

void MaskProcessor::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                 juce::MidiBuffer& midiBuffer) {
  if (!*bypass && audioBuffer.getNumChannels() > 0) {
    if (*fftOrderMenu != fftOrder - 7) changeOrder(*fftOrderMenu + 7);

    // the following lines are run per channel
    for (int chan = 0; chan < audioBuffer.getNumChannels(); chan++) {
      float* channelData = audioBuffer.getWritePointer(chan, 0);

      // the following lines are run per sample per channel
      for (int i = 0; i < audioBuffer.getNumSamples(); ++i) {
        // push sample into input buffers
        pushNextSampleIntoBuffers(channelData[i], chan);

        // copy next value from output queue into buffer
        channelData[i] = IObuffers[chan]->outputQueue[IObuffers[chan]->outputQueueIndex];
        IObuffers[chan]->outputQueue[IObuffers[chan]->outputQueueIndex] = 0;
        IObuffers[chan]->outputQueueIndex =
          (IObuffers[chan]->outputQueueIndex + 1) % IObuffers[chan]->outputQueue.size();
      }
    }

    // The following lines are run per buffer
    MakeUpFactor.applyGain(audioBuffer, audioBuffer.getNumSamples());
  }
}

//==============================================================================
// method for writing input samples into fft buffers
//==============================================================================
void MaskProcessor::pushNextSampleIntoBuffers(float sample, int chan) noexcept {
  // if the inputBuffer contains enough data, copy it to fftData.
  for (int i = 0; i < overlap; i++) {
    if (IObuffers[chan]->inputBufferIndex[i] >= fftSize) {
      mask(IObuffers[chan]->inputBuffer[i], chan);
      IObuffers[chan]->inputBufferIndex[i] = 0;
    }
    // write sample to next index of inputBuffer
    if (IObuffers[chan]->inputBufferIndex[i] >= 0)
      IObuffers[chan]->inputBuffer[i][(size_t)IObuffers[chan]->inputBufferIndex[i]] = sample;
    IObuffers[chan]->inputBufferIndex[i]++;
  }
}

//==============================================================================
// Do the actual Mask effect
//==============================================================================
void MaskProcessor::mask(std::vector<float>& buffer, int chan) {
  // get rms amplitude of input buffer for calculating required make-up gain later
  double rmsPost = 0;
  double rmsPre = 0;
  if (*makeUpGain) {
    for (int i = 0; i < fftSize; i++) rmsPre += pow(buffer[i], 2);
    rmsPre = sqrt(rmsPre / fftSize);
  }

  // window the samples
  juce::FloatVectorOperations::multiply(buffer.data(), window.data(), fftSize);

  // perform FFT in place, returns interleaved real-imaginary values
  forwardFFT->performRealOnlyForwardTransform(buffer.data(), true);

  // Manipulate bins

  // check if we should manipulate bins on this frame
  if (framecounter < maskChangeIntervalSlider->get()) {
    framecounter++;
  } else {
    framecounter = 0;

    // return masked bins to unmasked bins
    for (int i = 0; i < unmaskedBinsSlider->get() * (fftSize / 2); i++) {
      if (!maskedBins[chan].empty()) {
        maskedIt = maskedBins[chan].begin();
        std::advance(maskedIt, rand() % maskedBins[chan].size());
        *maskedIt = true;
        unmaskedBins[chan].splice(unmaskedBins[chan].begin(), maskedBins[chan], maskedIt);
      }
    }

    // add unmasked bins to masked bins
    for (int i = 0; i < maskedBinsSlider->get() * (fftSize / 2); i++) {
      if (!unmaskedBins[chan].empty()) {
        unmaskedIt = unmaskedBins[chan].begin();
        std::advance(unmaskedIt, rand() % unmaskedBins[chan].size());
        *unmaskedIt = false;
        maskedBins[chan].splice(maskedBins[chan].begin(), unmaskedBins[chan], unmaskedIt);
      }
    }
  }

  // mask the bins
  for (int i = 0; i < bins[chan].size(); i++) {
    if (!bins[chan][i]) {
      buffer[i * 2] = 0;
      buffer[(i * 2) + 1] = 0;
    }
  }

  // Inverse FFT
  inverseFFT->performRealOnlyInverseTransform(buffer.data());

  // calculate the required make-up gain
  if (*makeUpGain) {
    for (int i = 0; i < fftSize; i++) rmsPost += pow(buffer[i], 2);
    rmsPost = sqrt(rmsPost / fftSize);

    makeUpValuesWritePointer = (makeUpValuesWritePointer + 1) % makeUpValues.size();

    if (rmsPost > 0)
      makeUpValues[makeUpValuesWritePointer] = rmsPre / rmsPost;
    else
      makeUpValues[makeUpValuesWritePointer] = 1.0;
    MakeUpFactor.setTargetValue(std::accumulate(makeUpValues.begin(), makeUpValues.end(), 0.0f) /
                                makeUpValues.size());
  } else {
    MakeUpFactor.setTargetValue(1.0f);
  }

  for (int i = 0; i < fftSize; i++) {
    IObuffers[chan]->outputQueue[(IObuffers[chan]->outputQueueIndex + i) %
                                 IObuffers[chan]->outputQueue.size()] +=
      (buffer[i] * window[i]) / overlap;
  }
  juce::FloatVectorOperations::fill(buffer.data(), 0, buffer.size());
}

//==============================================================================
// Method for changing the order (size) of the fft
//==============================================================================
void MaskProcessor::changeOrder(int order) {
  fftOrder = order;
  fftSize = 1 << fftOrder;
  forwardFFT.reset(new juce::dsp::FFT(fftOrder));
  inverseFFT.reset(new juce::dsp::FFT(fftOrder));

  window.resize(fftSize + 1);
  juce::dsp::WindowingFunction<float>::fillWindowingTables(
    window.data(), fftSize + 1, juce::dsp::WindowingFunction<float>::WindowingMethod(windowIndex));
  hopSize = fftSize / overlap;
  for (int chan = 0; chan < IObuffers.size(); chan++) {
    for (int i = 0; i < overlap; i++) {
      IObuffers[chan]->inputBuffer[i].resize(fftSize * 2, 0.0f);
      IObuffers[chan]->inputBufferIndex[i] = 0 - hopSize * i;
    }
    IObuffers[chan]->outputQueue.resize(fftSize * 2, 0.0f);
    IObuffers[chan]->outputQueueIndex = 0;
    maskedBins[chan].clear();
    unmaskedBins[chan].clear();
    bins[chan].resize(fftSize / 2, 1);
    for (int i = 0; i < fftSize / 2; i++) unmaskedBins[chan].push_back(bins[chan][i]);
  }
}

//==============================================================================
// OSC stuff
//==============================================================================

void MaskProcessor::oscMessageReceived(const juce::OSCMessage& message) {
  if (*oscOn) {
    if (message.size() == 1 && message[0].isFloat32()) {
      bins[rand() % 2][(int)(((message[0].getFloat32() + 5.0f) / 10.0f) * bins[0].size())] = false;
    }
  }
}

//==============================================================================
// Plug-in stuff
//==============================================================================
bool MaskProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) return false;
#endif

  return true;
#endif
}
const juce::String MaskProcessor::getName() const {
#if JucePlugin_Build_VST3
  return JucePlugin_Name;
#else
  return JUCE_APPLICATION_NAME_STRING;
#endif
}

bool MaskProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool MaskProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool MaskProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double MaskProcessor::getTailLengthSeconds() const { return 0.0; }

int MaskProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0 programs,
             // so this should be at least 1, even if you're not really implementing programs.
}

int MaskProcessor::getCurrentProgram() { return 0; }

void MaskProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }

const juce::String MaskProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void MaskProcessor::changeProgramName(int index, const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void MaskProcessor::getStateInformation(juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void MaskProcessor::setStateInformation(const void* data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
  juce::ignoreUnused(data, sizeInBytes);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new MaskProcessor(); }
