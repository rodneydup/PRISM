
#ifndef DilateComponent_hpp
#define DilateComponent_hpp

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "BaseProcessor.hpp"

class DilateComponent : public BaseProcessor {
 public:
  DilateComponent();

  juce::AudioProcessorEditor* createEditor() override {
    return new juce::GenericAudioProcessorEditor(*this);
  }

  bool hasEditor() const override { return true; }

  ~DilateComponent();

  //==============================================================================
  void prepareToPlay(double, int) override;
  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================
  // Adding samples from the input stream into buffers
  void pushNextSampleIntoBuffers(float sample) noexcept;

  // FFT, transform, IFFT, and add to output queue a buffer that is ready
  void dilate(std::vector<std::complex<float>> buffer);

  // Change the fft Order/window size at runtime. Note that it
  // takes an ORDER, i.e. x where 2^x is the new window size.
  void changeOrder(int order);

  // Changing the window shape.
  void changeWindowType(int type);

  // Change number of overlapped stft windows. 1 is no overlap (discrete windows).
  void changeOverlap(int newOverlap);

  int fftOrder = 10;
  int fftSize = 1 << fftOrder;
  int windowIndex = 2;

 private:
  std::unique_ptr<juce::dsp::FFT> forwardFFT;
  std::unique_ptr<juce::dsp::FFT> inverseFFT;
  std::vector<float> window;
  std::vector<std::vector<std::complex<float>>> inputBuffer;
  std::vector<std::complex<float>> frequencyDomainData;
  std::vector<float> outputQueue;
  int outputQueueIndex = 0;
  int overlap = 2;
  int hopSize = fftSize / overlap;
  std::vector<int> inputBufferIndex;
  bool bypass = false;

  juce::AudioParameterChoice* fftOrderMenu;
  juce::AudioParameterChoice* fftWindowMenu;
  juce::AudioParameterInt* overlapSlider;
  juce::AudioParameterInt* focalPoint;
  juce::AudioParameterFloat* dilationFactor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DilateComponent)
};
#endif