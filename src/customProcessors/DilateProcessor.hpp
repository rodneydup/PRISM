
#ifndef DilateProcessor_hpp
#define DilateProcessor_hpp

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <fstream>
#include <iostream>

#include "../ringBuffer.hpp"

class DilateProcessor : public juce::AudioProcessor {
 public:
  DilateProcessor();

  juce::AudioProcessorEditor* createEditor() override {
    return new juce::GenericAudioProcessorEditor(*this);
  }

  bool hasEditor() const override { return true; }

  ~DilateProcessor();

  //==============================================================================
  void prepareToPlay(double, int) override;
  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  //==============================================================================
  // Adding samples from the input stream into buffers
  void pushNextSampleIntoBuffers(float sample, int chan) noexcept;

  // FFT, transform, IFFT, and add to output queue a buffer that is ready
  void dilate(std::vector<float>& buffer, int chan);

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

  struct IObuffer {
    std::vector<std::vector<float>> inputBuffer;
    std::vector<int> inputBufferIndex;
    std::vector<float> outputQueue;
    int outputQueueIndex = 0;
  };

 private:
  std::unique_ptr<juce::dsp::FFT> forwardFFT;
  std::unique_ptr<juce::dsp::FFT> inverseFFT;
  std::vector<float> window;
  std::vector<float> transformedData;
  std::vector<float> makeUpValues;
  int makeUpValuesWritePointer;
  // std::vector<std::unique_ptr<RingBuffer>> RMSpre;
  // std::vector<std::unique_ptr<RingBuffer>> RMSpost;

  std::vector<std::unique_ptr<IObuffer>> IObuffers;
  int overlap = 4;
  int hopSize = fftSize / overlap;

  juce::AudioParameterChoice* fftOrderMenu;
  juce::AudioParameterFloat* focalPointSlider;
  juce::AudioParameterFloat* dilationFactorSlider;
  juce::AudioParameterBool* bypass;
  juce::AudioParameterBool* makeUpGain;

  juce::SmoothedValue<float> focalPoint;
  juce::SmoothedValue<float> dilationFactor;

  juce::SmoothedValue<float> MakeUpFactor;

  float focalBin;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DilateProcessor)
};

#endif