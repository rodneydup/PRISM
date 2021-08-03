// To Do:
// Fix switching from stereo to mono
// crashes when mono plugin deleted
// fix makeup gain or just get rid of it (it's to the point where I'm basically implementing a full
// on compressor)
// look at KlangFalter code for auto makeup gain
// Make plugin remember parameters. ValueTree?

// FIX THE ALGORITHM:
// maybe stop trying to translate phase to new bins and just translate magnitude?
// Use Sinc function for transformed bins that land between bins
// phase will be necessary to calculate this bin smearing, right?
// figured out sinc interpolation of magnitude, but what about phase?

// should I rename this to Spectral homothety?

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
      IObuffers[chan]->inputBuffer[i].resize(fftSize * 2, 0.0f);
      IObuffers[chan]->inputBufferIndex[i] = 0 - hopSize * i;
    }
    IObuffers[chan]->outputQueue.resize(fftSize * 2, 0.0f);
  }
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
                 juce::AudioProcessorParameter::genericParameter, stringFromValue,
                 valueFromString));
  focalPoint[0].reset(2048);
  focalPoint[1].reset(2048);

  addParameter(dilationFactorSlider = new juce::AudioParameterFloat(
                 "dilationFactor", "Dilation Factor",
                 juce::NormalisableRange<float>(-2.0f, 2.0f, 0.0001), 1.0f, "Dilation Factor",
                 juce::AudioProcessorParameter::genericParameter, stringFromValue,
                 valueFromString));
  dilationFactor[0].reset(2048);
  dilationFactor[1].reset(2048);

  addParameter(makeUpGain = new juce::AudioParameterBool("makeupgain", "Auto Make-Up Gain", 0));
  addParameter(dilateAlgorithm =
                 new juce::AudioParameterBool("dilatealgorithm", "New dilate algorithm", 0));

  addParameter(bypass = new juce::AudioParameterBool("bypass", "Bypass", 0));

  MakeUpFactor.reset(2048);
  makeUpValues.resize(10, 0.0f);

  std::complex<float> thing1 = {0.5, 0.6};
  // std::cout < < < < std::endl;
}

DilateProcessor::~DilateProcessor() {}

//==============================================================================
void DilateProcessor::prepareToPlay(double, int) {}
void DilateProcessor::releaseResources() {}

void DilateProcessor::processBlock(juce::AudioBuffer<float>& audioBuffer,
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

        // iterate smoothvalues
        focalPoint[chan].getNextValue();
        dilationFactor[chan].getNextValue();
      }
      // set new targets for smoothvalues if slider changed
      if (focalPointSlider->get() != focalPoint[chan].getTargetValue())
        focalPoint[chan].setTargetValue(focalPointSlider->get());
      if (dilationFactorSlider->get() != dilationFactor[chan].getTargetValue())
        dilationFactor[chan].setTargetValue(dilationFactorSlider->get());
    }

    // The following lines are run per buffer
    MakeUpFactor.applyGain(audioBuffer, audioBuffer.getNumSamples());
  }
}

//==============================================================================
// method for writing input samples into fft buffers
//==============================================================================
void DilateProcessor::pushNextSampleIntoBuffers(float sample, int chan) noexcept {
  // if the inputBuffer contains enough data, copy it to fftData.
  for (int i = 0; i < overlap; i++) {
    if (IObuffers[chan]->inputBufferIndex[i] >= fftSize) {
      dilate(IObuffers[chan]->inputBuffer[i], chan);
      IObuffers[chan]->inputBufferIndex[i] = 0;
    }
    // write sample to next index of inputBuffer
    if (IObuffers[chan]->inputBufferIndex[i] >= 0)
      IObuffers[chan]->inputBuffer[i][(size_t)IObuffers[chan]->inputBufferIndex[i]] = sample;
    IObuffers[chan]->inputBufferIndex[i]++;
  }
}

//==============================================================================
// Do the actual Dilate effect
//==============================================================================
void DilateProcessor::dilate(std::vector<float>& buffer, int chan) {
  // focalBin is the focalFreq converted to units of bins. Used so much it's worth precalculating.
  // (sr / fftSize is the bin size)
  focalBin = focalPoint[chan].getCurrentValue() / (getSampleRate() / fftSize);

  // get rms amplitude of input buffer for calculating required make-up gain later
  double rmsPost = 0;
  double rmsPre = 0;
  if (*makeUpGain) {
    for (int i = 0; i < fftSize; i++) rmsPre += pow(buffer[i], 2);
    rmsPre = sqrt(rmsPre / fftSize);
  }

  // float peakPre = 0;
  // float peakPost = 0;
  // if (*makeUpGain) {
  //   for (int i = 0; i < fftSize; i++) peakPre = juce::jmax(peakPre, buffer[i]);
  // }

  // window the samples
  juce::FloatVectorOperations::multiply(buffer.data(), window.data(), fftSize);

  // perform FFT in place, returns interleaved real-imaginary values
  forwardFFT->performRealOnlyForwardTransform(buffer.data(), true);

  // Manipulate bins
  for (int preTransformedBin = 1; preTransformedBin < fftSize / 2; preTransformedBin++) {
    // where to move the energy of this bin
    float transformedIndex =
      ((preTransformedBin - focalBin) * (dilationFactor[chan].getCurrentValue())) + focalBin;

    // if the result ends up higher than nyquist or below zero, skip it
    if (transformedIndex < fftSize / 2 && transformedIndex > 1) {
      if (*dilateAlgorithm) {
        // iterate over a range of bins near the result to account for spectral leakage
        for (int interpBin = floor(transformedIndex - 4); interpBin <= floor(transformedIndex + 4);
             interpBin++) {
          if (interpBin > 0 && interpBin < fftSize / 2) {
            const float magSource = sqrt(pow(buffer[preTransformedBin * 2], 2) +
                                         pow(buffer[preTransformedBin * 2 + 1], 2));
            const float phaseSource =
              atan2(buffer[preTransformedBin * 2 + 1], buffer[preTransformedBin * 2]);
            const float phaseTarget = atan2(buffer[interpBin * 2 + 1], buffer[interpBin * 2]);

            // interpolate with a sinc function
            const float interpScalar = sincInterpolation(transformedIndex - interpBin);

            // const float phase = sin(2 * M_PI * transformedIndex * interpBin);

            // // real
            // transformedData[interpBin * 2] += buffer[preTransformedBin * 2] * interpScalar;
            // // imaginary
            // transformedData[interpBin * 2 + 1] += buffer[preTransformedBin * 2 + 1] *
            // interpScalar;

            // real
            transformedData[interpBin * 2] += cos(phaseTarget) * magSource * interpScalar;
            // imaginary
            transformedData[interpBin * 2 + 1] += sin(phaseTarget) * magSource * interpScalar;
          }
        }
      } else {
        // bin splitting for transformed indexes between bins
        const unsigned j = floor(transformedIndex) * 2;
        const unsigned k = j + 2;
        const float t = transformedIndex - floor(transformedIndex);
        // // real part Accumulation
        transformedData[j] += buffer[preTransformedBin * 2] * (1 - t);
        transformedData[k] += buffer[preTransformedBin * 2] * t;
        // // imaginary part Accumulation
        transformedData[j + 1] += buffer[preTransformedBin * 2 + 1] * (1 - t);
        transformedData[k + 1] += buffer[preTransformedBin * 2 + 1] * t;
      }
    }
  }

  // Inverse FFT
  inverseFFT->performRealOnlyInverseTransform(transformedData.data());

  // calculate the required make-up gain
  if (*makeUpGain) {
    for (int i = 0; i < fftSize; i++) rmsPost += pow(transformedData[i], 2);
    rmsPost = sqrt(rmsPost / fftSize);

    // for (int i = 0; i < fftSize; i++) peakPost = juce::jmax(peakPost, transformedData[i]);

    makeUpValuesWritePointer = (makeUpValuesWritePointer + 1) % makeUpValues.size();
    if (rmsPost > 0)
      makeUpValues[makeUpValuesWritePointer] = rmsPre / rmsPost;
    else
      makeUpValues[makeUpValuesWritePointer] = 1.0;
    MakeUpFactor.setTargetValue(std::accumulate(makeUpValues.begin(), makeUpValues.end(), 0.0f) /
                                makeUpValues.size());
    // if (peakPost > 0) MakeUpFactor = peakPre / peakPost;
  } else {
    MakeUpFactor.setTargetValue(1.0f);
    // MakeUpFactor = 1.0f;
  }

  for (int i = 0; i < fftSize; i++) {
    IObuffers[chan]->outputQueue[(IObuffers[chan]->outputQueueIndex + i) %
                                 IObuffers[chan]->outputQueue.size()] +=
      (transformedData[i] * window[i]) / overlap;
  }
  juce::FloatVectorOperations::fill(transformedData.data(), 0, transformedData.size());
}

//==============================================================================
// Method for changing the order (size) of the fft
//==============================================================================
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
//==============================================================================
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
