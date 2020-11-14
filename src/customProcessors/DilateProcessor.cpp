// To Do:
// figure out phase??!!
// Fix witching from stereo to mono

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
  for (int i = 0; i < 2; i++) {
    IObuffers.push_back(std::make_unique<IObuffer>());
    // RMSpre.push_back(std::make_unique<RingBuffer>(4096));
    // RMSpost.push_back(std::make_unique<RingBuffer>(4096));
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
  }
  // frequencyDomainData.resize(fftSize, 0.0f);
  transformedData.resize(fftSize * 2, 0.0f);

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
  addParameter(makeUpGain = new juce::AudioParameterBool("makeupgain", "Auto Make-Up Gain", 0));
  addParameter(bypass = new juce::AudioParameterBool("bypass", "Bypass", 0));

  MakeUpFactor.reset(4096);
  makeUpValues.resize(40, 0.0f);
}

DilateProcessor::~DilateProcessor() {}

//==============================================================================
void DilateProcessor::prepareToPlay(double, int) {}
void DilateProcessor::releaseResources() {}

void DilateProcessor::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                   juce::MidiBuffer& midiBuffer) {
  if (!*bypass) {
    if (audioBuffer.getNumChannels() > 0) {
      // float RMScalculatedPre = 0.0f;
      // float RMScalculatedPost = 0.0f;
      if (*fftOrderMenu != fftOrder - 7) changeOrder(*fftOrderMenu + 7);
      for (int chan = 0; chan < audioBuffer.getNumChannels(); chan++) {
        float* channelData = audioBuffer.getWritePointer(chan, 0);
        for (int i = 0; i < audioBuffer.getNumSamples(); ++i) {
          // push sample into input buffers
          pushNextSampleIntoBuffers(channelData[i], chan);

          // if (*makeUpGain) {
          //   RMSpre[chan]->push_back(channelData[i]);
          //   RMSpost[chan]->push_back(
          //     IObuffers[chan]->outputQueue[IObuffers[chan]->outputQueueIndex]);
          // }

          // copy next value from output queue into buffer
          channelData[i] = IObuffers[chan]->outputQueue[IObuffers[chan]->outputQueueIndex];
          IObuffers[chan]->outputQueue[IObuffers[chan]->outputQueueIndex] = 0;
          IObuffers[chan]->outputQueueIndex =
            (IObuffers[chan]->outputQueueIndex + 1) % IObuffers[chan]->outputQueue.size();
          if (chan == 0) {
            // per sample but not per channel
            // iterate smoothvalues
            focalPoint.getNextValue();
            dilationFactor.getNextValue();
          }
        }
        // find highest rms value on any channel
        // if (*makeUpGain) {
        //   RMScalculatedPre = juce::jmax(RMSpre[chan]->getRMS(4096), RMScalculatedPre);
        //   RMScalculatedPost = juce::jmax(RMSpost[chan]->getRMS(4096), RMScalculatedPost);
        // }
      }
      // per buffer
      MakeUpFactor.applyGain(audioBuffer, audioBuffer.getNumSamples());
      // set new targets for smoothvalues if slider changed
      if (focalPointSlider->get() != focalPoint.getTargetValue())
        focalPoint.setTargetValue(focalPointSlider->get());
      if (dilationFactorSlider->get() != dilationFactor.getTargetValue())
        dilationFactor.setTargetValue(dilationFactorSlider->get());
      // calculate make-up gain
      // if (*makeUpGain) {
      //   if (RMScalculatedPost > 0)
      //     MakeUpFactor.setTargetValue(RMScalculatedPre / RMScalculatedPost);
      // } else {
      //   MakeUpFactor.setTargetValue(1.0f);
      // }
    }
  }
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

void DilateProcessor::dilate(std::vector<float>& buffer, int chan) {
  // focalBin is the focalFreq converted to units of bins. Used so much it's worth precalculating.
  focalBin = (focalPoint.getCurrentValue() / (getSampleRate() / fftSize));

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
  // Manipulate bins ========================================================
  for (int i = 0; i < (fftSize / 2) + 1; i++) {
    float transformedIndex = ((i - focalBin) * (dilationFactor.getCurrentValue())) + focalBin;
    if (transformedIndex < (fftSize / 2) - 2 && transformedIndex > 2) {
      // bin splitting for transformed indexes between bins
      const unsigned j = floor(transformedIndex) * 2;
      const unsigned k = j + 2;
      const float t = transformedIndex - floor(transformedIndex);
      // Magnitude Accumulation
      transformedData[j] += buffer[i * 2] * (1 - t);
      transformedData[k] += buffer[i * 2] * t;
      // Phase Accumulation
      transformedData[j + 1] += buffer[(i * 2) + 1] * (1 - t);
      transformedData[k + 1] += buffer[(i * 2) + 1] * t;
    }
  }

  // ========================================================================

  // Inverse FFT
  inverseFFT->performRealOnlyInverseTransform(transformedData.data());
  if (*makeUpGain) {
    for (int i = 0; i < fftSize; i++) rmsPost += pow(transformedData[i], 2);
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
      (transformedData[i] * window[i]) / overlap;
  }
  juce::FloatVectorOperations::fill(transformedData.data(), 0, transformedData.size());
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
      IObuffers[chan]->inputBuffer[i].resize(fftSize * 2, 0.0f);
      IObuffers[chan]->inputBufferIndex[i] = 0 - hopSize * i;
    }
    IObuffers[chan]->outputQueue.resize(fftSize * 2, 0.0f);
    IObuffers[chan]->outputQueueIndex = 0;
  }
  transformedData.resize(fftSize * 2, 0.0f);
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
