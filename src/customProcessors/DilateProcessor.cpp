// To Do:
// figure out phase??!!
//

#include "DilateProcessor.hpp"

DilateProcessor::DilateProcessor()
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
      IObuffers[chan]->inputBuffer[i].resize(forwardFFT->getSize(), 0.0f);
      IObuffers[chan]->inputBufferIndex[i] = 0 - hopSize * i;
    }
    IObuffers[chan]->outputQueue.resize(fftSize * 2, 0.0f);
  }
  frequencyDomainData.resize(fftSize, 0.0f);
  transformedData.resize(fftSize, 0.0f);

  // add parameters
  addParameter(fftOrderMenu = new juce::AudioParameterChoice(
                 "fftOrderMenu", "FFT Order", {"128", "256", "512", "1024", "2048", "4096", "8192"},
                 fftOrder - 7, "FFT Order"));
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

DilateProcessor::~DilateProcessor() {}

//==============================================================================
void DilateProcessor::prepareToPlay(double, int) {}
void DilateProcessor::releaseResources() {}

void DilateProcessor::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                   juce::MidiBuffer& midiBuffer) {
  if (audioBuffer.getNumChannels() > 0) {
    if (*fftOrderMenu != fftOrder - 7) changeOrder(*fftOrderMenu + 7);
    for (int chan = 0; chan < audioBuffer.getNumChannels(); chan++) {
      float* channelData = audioBuffer.getWritePointer(chan, 0);
      for (int i = 0; i < audioBuffer.getNumSamples(); ++i) {
        // push sample into input buffers
        pushNextSampleIntoBuffers(channelData[i], chan);

        // copy next value from output queue into buffer
        channelData[i] = IObuffers[chan]->outputQueue[IObuffers[chan]->outputQueueIndex];
        IObuffers[chan]->outputQueue[IObuffers[chan]->outputQueueIndex] = 0;
        IObuffers[chan]->outputQueueIndex =
          (IObuffers[chan]->outputQueueIndex + 1) % IObuffers[chan]->outputQueue.size();
        if (chan == 0) {
          // iterate smoothvalues per sample but not per channel
          focalPoint.getNextValue();
          dilationFactor.getNextValue();
        }
      }
    }
  }
  // set new targets for smoothvalues per buffer if slider changed
  if (focalPointSlider->get() != focalPoint.getTargetValue())
    focalPoint.setTargetValue(focalPointSlider->get());
  if (dilationFactorSlider->get() != dilationFactor.getTargetValue())
    dilationFactor.setTargetValue(dilationFactorSlider->get());
}

//==============================================================================
void DilateProcessor::pushNextSampleIntoBuffers(float sample, int chan) noexcept {
  // if the inputBuffer contains enough data, copy it to fftData.
  for (int i = 0; i < overlap; i++) {
    if (IObuffers[chan]->inputBufferIndex[i] == fftSize) {
      dilate(IObuffers[chan]->inputBuffer[i], chan);
      IObuffers[chan]->inputBufferIndex[i] = 0;
    }
    // write sample to next index of inputBuffer
    if (IObuffers[chan]->inputBufferIndex[i] >= 0)
      IObuffers[chan]->inputBuffer[i][(size_t)IObuffers[chan]->inputBufferIndex[i]] = sample;
    IObuffers[chan]->inputBufferIndex[i]++;
    // std::cout << inputBufferIndex[i] << std::endl;
  }
}

void DilateProcessor::dilate(std::vector<std::complex<float>>& buffer, int chan) {
  // focalBin is the focalFreq converted to units of bins. Used so much it's worth precalculating.
  focalBin = (focalPoint.getCurrentValue() / (getSampleRate() / fftSize));

  // window the samples
  for (int i = 0; i < fftSize; i++) buffer[i] *= window[i];
  // perform FFT (would in-place be faster? realonly?)
  forwardFFT->perform(buffer.data(), frequencyDomainData.data(), false);
  // Manipulate bins ========================================================
  for (int i = 0; i < (fftSize / 2) + 1; i++) {
    float transformedIndex = ((i - focalBin) * (dilationFactor.getCurrentValue())) + focalBin;
    if (transformedIndex < (fftSize / 2) && transformedIndex > 0) {
      // bin splitting for transformed indexes between bins
      const unsigned j = floor(transformedIndex);
      const unsigned k = j + 1;
      const float t = transformedIndex - j;
      // add magnitudes
      transformedData[j].real(transformedData[j].real() +
                              abs(frequencyDomainData[i].real() * (1 - t)));
      transformedData[k].real(transformedData[k].real() + abs(frequencyDomainData[i].real() * t));
      // add phases
      transformedData[j].imag(transformedData[j].imag() +
                              (frequencyDomainData[i].imag() * (1 - t)));
      transformedData[k].imag(transformedData[k].imag() + (frequencyDomainData[i].imag() * t));
    }
  }
  // ========================================================================
  for (int i = (fftSize / 2) + 1; i < fftSize; i++) {  // DFT mirroring semantics
    transformedData[i].real(transformedData[fftSize - i].real());
    transformedData[i].imag(-1 * transformedData[fftSize - i].imag());
  }
  // Inverse FFT
  inverseFFT->perform(transformedData.data(), buffer.data(), true);
  for (int i = 0; i < fftSize; i++) {
    IObuffers[chan]->outputQueue[(IObuffers[chan]->outputQueueIndex + i) %
                                 IObuffers[chan]->outputQueue.size()] +=
      buffer[i].real() * window[i] / overlap;
    transformedData[i] = std::complex<float>(0.0f, 0.0f);
    frequencyDomainData[i] = std::complex<float>(0.0f, 0.0f);
  }
}

void DilateProcessor::changeOrder(int order) {
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
      IObuffers[chan]->inputBuffer[i].resize(fftSize, 0.0f);
      IObuffers[chan]->inputBufferIndex[i] = 0 - hopSize * i;
    }
    IObuffers[chan]->outputQueue.resize(fftSize * 2, 0.0f);
    IObuffers[chan]->outputQueueIndex = 0;
  }
  frequencyDomainData.resize(fftSize, 0.0f);
  transformedData.resize(fftSize, 0.0f);
}

//==============================================================================
// Plug-in stuff

bool DilateProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
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
const juce::String DilateProcessor::getName() const {
#if JucePlugin_Build_VST3
  return JucePlugin_Name;
#else
  return JUCE_APPLICATION_NAME_STRING;
#endif
}

bool DilateProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool DilateProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool DilateProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double DilateProcessor::getTailLengthSeconds() const { return 0.0; }

int DilateProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0 programs,
             // so this should be at least 1, even if you're not really implementing programs.
}

int DilateProcessor::getCurrentProgram() { return 0; }

void DilateProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }

const juce::String DilateProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void DilateProcessor::changeProgramName(int index, const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void DilateProcessor::getStateInformation(juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void DilateProcessor::setStateInformation(const void* data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
  juce::ignoreUnused(data, sizeInBytes);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new DilateProcessor(); }
