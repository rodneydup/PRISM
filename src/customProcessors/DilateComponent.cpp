// To Do:
// - Windowing causes amplitude modulation. I see why but does that mean I'm doing something
// wrong? Do I have to do windowing again or some kind of "unwindowing" at the ifft?
// - fft sizes smaller than buffer size has audio artifacts. Investigate
// - Finish tests to make sure fft and ifft aren't causing audio glitches
// - Write dilation algorithm
// - Hook up focalPoint and dilationFactor sliders

#include "DilateComponent.hpp"

DilateComponent::DilateComponent() {
  forwardFFT = std::make_unique<juce::dsp::FFT>(fftOrder);
  inverseFFT = std::make_unique<juce::dsp::FFT>(fftOrder);
  window.resize(fftSize);
  juce::dsp::WindowingFunction<float>::fillWindowingTables(
    window.data(), fftSize, juce::dsp::WindowingFunction<float>::WindowingMethod(windowIndex));
  inputBuffer.resize(overlap);
  inputBufferIndex.resize(overlap);
  for (int i = 0; i < overlap; i++) {
    inputBuffer[i].resize(fftSize, 0.0f);
    inputBufferIndex[i] = 0 - hopSize * i;
  }
  frequencyDomainData.resize(fftSize, 0.0f);
  transformedData.resize(fftSize, 0.0f);

  outputQueue.resize(fftSize * 2, 0.0f);

  addParameter(fftOrderMenu = new juce::AudioParameterChoice(
                 "fftOrderMenu", "FFT Order", {"64", "128", "256", "512", "1024", "2048", "4096"},
                 fftOrder - 6, "FFT Order"));
  juce::StringArray juceWindows;
  for (int i = 0; i < juce::dsp::WindowingFunction<float>::numWindowingMethods; i++)
    juceWindows.add(juce::dsp::WindowingFunction<float>::getWindowingMethodName(
      juce::dsp::WindowingFunction<float>::WindowingMethod(i)));
  addParameter(fftWindowMenu = new juce::AudioParameterChoice(
                 "fftWindowMenu", "FFT Window", juceWindows, windowIndex, "FFT Window"));
  addParameter(overlapSlider = new juce::AudioParameterInt("overlap", "Overlap", 1, 12, 2));
  addParameter(focalPointSlider = new juce::AudioParameterFloat(
                 "focalPoint", "Focal Point",
                 juce::NormalisableRange<float>(0.0f, 10000.0f, 0.1, 0.4), 400.0f));
  focalPoint.reset(1024);

  addParameter(dilationFactorSlider = new juce::AudioParameterFloat(
                 "dilationFactor", "Dilation Factor",
                 juce::NormalisableRange<float>(-2.0f, 2.0f, 0.001), 1.0f));
  dilationFactor.reset(1024);

  addParameter(bypass = new juce::AudioParameterBool("bypass", "Bypass", 0));
}

DilateComponent::~DilateComponent() {}

//==============================================================================
void DilateComponent::prepareToPlay(double, int) {}
void DilateComponent::releaseResources() {}

void DilateComponent::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                   juce::MidiBuffer& midiBuffer) {
  if (!*bypass) {
    if (audioBuffer.getNumChannels() > 0) {
      if (*fftWindowMenu != windowIndex) changeWindowType(*fftWindowMenu);
      if (*fftOrderMenu != fftOrder - 6) changeOrder(*fftOrderMenu + 6);
      if (*overlapSlider != overlap) changeOverlap(*overlapSlider);
      float* channelData = audioBuffer.getWritePointer(0, 0);
      for (int i = 0; i < audioBuffer.getNumSamples(); ++i) {
        // push sample into input buffers
        pushNextSampleIntoBuffers(channelData[i]);

        // copy next value from output queue into buffer
        channelData[i] = outputQueue[outputQueueIndex];
        outputQueue[outputQueueIndex] = 0;
        outputQueueIndex = (outputQueueIndex + 1) % outputQueue.size();

        // iterate smoothvalues per sample
        focalPoint.getNextValue();
        dilationFactor.getNextValue();
      }
    }
    // set new targets for smoothvalues per buffer if slider changed
    if (focalPointSlider->get() != focalPoint.getTargetValue())
      focalPoint.setTargetValue(focalPointSlider->get());
    if (dilationFactorSlider->get() != dilationFactor.getTargetValue())
      dilationFactor.setTargetValue(dilationFactorSlider->get());
  }
}
//==============================================================================

void DilateComponent::pushNextSampleIntoBuffers(float sample) noexcept {
  // if the inputBuffer contains enough data, copy it to fftData.
  for (int i = 0; i < overlap; i++) {
    if (inputBufferIndex[i] == fftSize) {
      dilate(inputBuffer[i]);
      inputBufferIndex[i] = 0;
    }
    // write sample to next index of inputBuffer
    if (inputBufferIndex[i] >= 0) inputBuffer[i][(size_t)inputBufferIndex[i]] = sample;
    inputBufferIndex[i]++;
    // std::cout << inputBufferIndex[i] << std::endl;
  }
}

void DilateComponent::dilate(std::vector<std::complex<float>> buffer) {
  // focalBin is the focalFreq converted to units of bins. Used so much it's worth precalculating.
  focalBin = (focalPoint.getCurrentValue() / (getSampleRate() / fftSize));

  // window the samples
  for (int i = 0; i < fftSize; i++) buffer[i] *= window[i];
  // perform FFT (would in-place be faster? realonly?)
  forwardFFT->perform(buffer.data(), frequencyDomainData.data(), false);
  // Manipulate bins ========================================================
  for (int i = 0; i < fftSize / 2; i++) {
    float transformedIndex = ((i - focalBin) * (dilationFactor.getCurrentValue())) + focalBin;
    if (transformedIndex < (fftSize / 2) && transformedIndex > 0) {
      const unsigned j = floor(transformedIndex);
      const unsigned k = j + 1;
      const float t = transformedIndex - j;
      transformedData[j] += frequencyDomainData[i] * (1 - t);
      transformedData[k] += frequencyDomainData[i] * t;
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
    outputQueue[(outputQueueIndex + i) % outputQueue.size()] += buffer[i].real() / overlap;
    transformedData[i] = (0, 0);
  }
}

void DilateComponent::changeOrder(int order) {
  // bypass to make sure we don't write new samples while the arrays are being resized.
  fftOrder = order;
  fftSize = 1 << fftOrder;
  forwardFFT.reset(new juce::dsp::FFT(fftOrder));
  inverseFFT.reset(new juce::dsp::FFT(fftOrder));

  window.resize(fftSize);
  juce::dsp::WindowingFunction<float>::fillWindowingTables(
    window.data(), fftSize, juce::dsp::WindowingFunction<float>::WindowingMethod(windowIndex),
    true);
  hopSize = fftSize / overlap;
  for (int i = 0; i < overlap; i++) {
    inputBuffer[i].resize(fftSize, 0.0f);
    inputBufferIndex[i] = 0 - hopSize * i;
  }
  frequencyDomainData.resize(fftSize, 0.0f);
  transformedData.resize(fftSize, 0.0f);

  outputQueue.resize(fftSize * 2, 0.0f);
  outputQueueIndex = 0;
}

void DilateComponent::changeWindowType(int type) {
  windowIndex = type;
  juce::dsp::WindowingFunction<float>::fillWindowingTables(
    window.data(), fftSize, juce::dsp::WindowingFunction<float>::WindowingMethod(windowIndex),
    true);
}

void DilateComponent::changeOverlap(int newOverlap) {
  overlap = newOverlap;
  hopSize = fftSize / overlap;
  inputBuffer.resize(overlap);
  inputBufferIndex.resize(overlap);
  for (int i = 0; i < overlap; i++) {
    inputBuffer[i].resize(fftSize * 2, 0.0f);
    inputBufferIndex[i] = 0 - hopSize * i;
  }
}