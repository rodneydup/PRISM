#include "DilateComponent.hpp"

DilateComponent::DilateComponent() {
  forwardFFT = std::make_unique<juce::dsp::FFT>(fftOrder);
  window = std::make_unique<juce::dsp::WindowingFunction<float>>(
    fftSize, juce::dsp::WindowingFunction<float>::WindowingMethod(windowType));
  fifo.resize(fftSize);
  fftData.resize(fftSize * 2);
  std::cout << fftSize << std::endl;

  addParameter(fftOrderMenu = new juce::AudioParameterChoice(
                 "fftOrderMenu", "FFT Order", {"64", "128", "256", "512", "1024", "2048", "4096"},
                 4, "FFT Order"));
  juce::StringArray juceWindows;
  for (int i = 0; i < juce::dsp::WindowingFunction<float>::numWindowingMethods; i++)
    juceWindows.add(juce::dsp::WindowingFunction<float>::getWindowingMethodName(
      juce::dsp::WindowingFunction<float>::WindowingMethod(i)));
  addParameter(fftWindowMenu = new juce::AudioParameterChoice("fftWindowMenu", "FFT Window",
                                                              juceWindows, 2, "FFT Window"));
  addParameter(focalPoint =
                 new juce::AudioParameterInt("focalPoint", "Focal Point", 0, 10000, 400));
  addParameter(dilationFactor = new juce::AudioParameterFloat("dilationFactor", "Dilation Factor",
                                                              -2.0f, 2.0f, 1.0f));
}

DilateComponent::~DilateComponent() {}

//==============================================================================
void DilateComponent::prepareToPlay(double, int) {}
void DilateComponent::releaseResources() {}

void DilateComponent::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                   juce::MidiBuffer& midiBuffer) {
  if (!bypass) {  // bypass variable stops from writing to fft arrays while they are being resized
    if (audioBuffer.getNumChannels() > 0) {
      float* channelData = audioBuffer.getWritePointer(0, 0);
      for (auto i = 0; i < audioBuffer.getNumSamples(); ++i) pushNextSampleIntoFifo(channelData[i]);
      for (int i = 0; i < audioBuffer.getNumSamples(); i++) {
        if (!outputQueue.empty()) {
          channelData[i] = outputQueue.front();
          outputQueue.pop();
        }
      }
    }
  }
}

//==============================================================================

void DilateComponent::pushNextSampleIntoFifo(float sample) noexcept {
  // if the fifo contains enough data, copy it to fftData. IS THIS A BAD IDEA?? This function gets
  // called in the audio thread... Though this copy only happens every "fftSize" samples.
  if (fifoIndex == fftSize) {
    std::fill(fftData.begin(), fftData.end(), 0.0f);
    std::copy(fifo.begin(), fifo.end(), fftData.begin());
    dilate();

    fifoIndex = 0;
  }
  // write sample to next index of fifo
  fifo[(size_t)fifoIndex++] = sample;
}

void DilateComponent::dilate() {
  // window the samples
  window->multiplyWithWindowingTable(fftData.data(), fftSize);
  // perform FFT
  forwardFFT->performRealOnlyForwardTransform(fftData.data());
  // Manipulate bins

  // Inverse FFT
  forwardFFT->performRealOnlyInverseTransform(fftData.data());
  for (int i = 0; i < fftSize; i++) outputQueue.push(fftData[i]);
}

void DilateComponent::changeOrder(int order) {
  // bypass to make sure we don't write new samples while the arrays are being resized.
  bypass = true;
  int fftOrder = order;
  int fftSize = 1 << fftOrder;
  forwardFFT.reset(new juce::dsp::FFT(fftOrder));
  fifo.resize(fftSize);
  fftData.resize(fftSize * 2);
  window.reset(new juce::dsp::WindowingFunction<float>(
    fftSize, juce::dsp::WindowingFunction<float>::WindowingMethod(windowType)));
  bypass = false;
}

void DilateComponent::changeWindowType(int type) {
  bypass = true;
  windowType = type;
  window.reset(new juce::dsp::WindowingFunction<float>(
    fftSize, juce::dsp::WindowingFunction<float>::WindowingMethod(windowType)));
  bypass = false;
}
