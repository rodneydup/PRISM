

#ifndef SpectrogramComponent_hpp
#define SpectrogramComponent_hpp

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "BaseProcessor.hpp"

class SpectrogramComponent : public BaseProcessor {
 public:
  SpectrogramComponent();

  juce::AudioProcessorEditor* createEditor() override { return new Editor(*this); }

  bool hasEditor() const override { return true; }

  ~SpectrogramComponent();

  //==============================================================================
  void prepareToPlay(double, int) override;
  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================

  void pushNextSampleIntoFifo(float sample) noexcept;

  void drawNextLineOfSpectrogram();

  void changeOrder(int order);

  int fftOrder = 10;
  int fftSize = 1 << fftOrder;

  juce::Image spectrogramImage;

 private:
  std::unique_ptr<juce::dsp::FFT> forwardFFT;
  std::vector<float> fifo;
  std::vector<float> fftData;
  int fifoIndex = 0;
  bool nextFFTBlockReady = false;
  bool bypass = false;

  class Editor : public juce::AudioProcessorEditor {
   public:
    Editor(SpectrogramComponent& owner);
    ~Editor();

    void paint(juce::Graphics& g) override;

    juce::ComboBox fftOrderMenu;

   private:
    juce::Image* spectrogramImage;
  };
};
#endif