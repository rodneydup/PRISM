
#ifndef DilateComponent_hpp
#define DilateComponent_hpp

// get access to juce::AudioProcessor
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>

#include "BaseProcessor.hpp"

class DilateComponent : public BaseProcessor {
 public:
  DilateComponent();

  //==============================================================================
  void prepareToPlay(double, int) override;
  void releaseResources() override;
  void processBlock(juce::AudioBuffer<float>& audioBuffer, juce::MidiBuffer& midiBuffer) override;

  static constexpr auto fftOrder = 10;
  static constexpr auto fftSize = 1 << fftOrder;

 private:
  //==============================================================================
  juce::dsp::FFT forwardFFT;
  std::array<float, fftSize> fifo;
  std::array<float, fftSize * 2> fftData;
  int fifoIndex = 0;
  bool nextFFTBlockReady = false;
  juce::Image spectrogramImage;
};
#endif