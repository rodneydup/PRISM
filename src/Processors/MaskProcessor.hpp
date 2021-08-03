
#ifndef MaskProcessor_hpp
#define MaskProcessor_hpp

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_osc/juce_osc.h>

#include <fstream>
#include <iostream>

class MaskProcessor
  : public juce::AudioProcessor,
    private juce::OSCReceiver,
    juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback> {
 public:
  MaskProcessor();

  juce::AudioProcessorEditor* createEditor() override {
    auto editor = new juce::GenericAudioProcessorEditor(*this);
    editor->setResizeLimits(200, 200, 1500, 1000);
    editor->setResizable(true, true);
    editor->setSize(600, 300);
    return editor;
  }

  bool hasEditor() const override { return true; }

  ~MaskProcessor();

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

  // FFT, edit bins, IFFT, and add to output queue a buffer that is ready
  void mask(std::vector<float>& buffer, int chan);

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
  std::vector<float> makeUpValues;
  int makeUpValuesWritePointer;

  std::vector<std::unique_ptr<IObuffer>> IObuffers;
  int overlap = 4;
  int hopSize = fftSize / overlap;

  juce::AudioParameterChoice* fftOrderMenu;
  juce::AudioParameterBool* bypass;
  juce::AudioParameterBool* makeUpGain;
  juce::AudioParameterBool* oscOn;
  juce::AudioParameterFloat* maskedBinsSlider;
  juce::AudioParameterFloat* unmaskedBinsSlider;
  juce::AudioParameterInt* maskChangeIntervalSlider;
  std::unique_ptr<juce::Label> oscAddress;

  std::vector<bool> bins[2];
  std::list<std::vector<bool>::reference> unmaskedBins[2];
  std::list<std::vector<bool>::reference> maskedBins[2];
  std::list<std::vector<bool>::reference>::iterator maskedIt;
  std::list<std::vector<bool>::reference>::iterator unmaskedIt;

  int framecounter = 0;

  juce::SmoothedValue<float> MakeUpFactor;
  // float MakeUpFactor = 1.0;

  std::function<juce::String(float value, int maximumStringLength)> stringFromValue =
    [&](float value, int maximumStringLength) {
      value = value * (fftSize / 2);
      std::ostringstream out;
      out << std::fixed << std::setprecision(0) << value;
      return out.str();
    };

  std::function<float(const juce::String& text)> valueFromString = [&](const juce::String& text) {
    return text.getFloatValue() / (fftSize / 2);
  };

  std::function<juce::String(int value, int maximumStringLength)> stringFromValueInt =
    [](int value, int maximumStringLength) {
      std::ostringstream out;
      out << value;
      return out.str();
    };

  std::function<int(const juce::String& text)> valueFromStringInt = [](const juce::String& text) {
    return text.getIntValue();
  };

  //===================================== OSC
  void oscMessageReceived(const juce::OSCMessage& message) override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MaskProcessor)
};

#endif