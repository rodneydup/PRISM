
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
  // this is modified from the juce fft tutorial
  void pushNextSampleIntoFifo(float sample) noexcept;

  // this is modified from the juce fft tutorial
  void dilate();

  // I added this function to allow changing the fft Order/window size at runtime. Note that it
  // takes an ORDER, i.e. x where 2^x is the new window size.
  void changeOrder(int order);

  // I added this function to allow changing the window shape at runtime.
  void changeWindowType(int type);

  int fftOrder = 10;
  int fftSize = 1 << fftOrder;
  int windowType = 2;

 private:
  std::unique_ptr<juce::dsp::FFT> forwardFFT;
  std::unique_ptr<juce::dsp::WindowingFunction<float>> window;
  std::vector<float> fifo;
  std::vector<float> fftData;
  std::queue<float> outputQueue;
  int fifoIndex = 0;
  bool bypass = false;

  juce::AudioParameterChoice* fftOrderMenu;
  juce::AudioParameterChoice* fftWindowMenu;
  juce::AudioParameterInt* focalPoint;
  juce::AudioParameterFloat* dilationFactor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DilateComponent)
};
#endif