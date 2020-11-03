
#ifndef PluginProcessor_hpp
#define PluginProcessor_hpp

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <fstream>
#include <iostream>

#include "customProcessors/DilateProcessor.hpp"

class PluginProcessor : public juce::AudioProcessor {
 public:
  PluginProcessor();

  juce::AudioProcessorEditor* createEditor() override {
    return new juce::GenericAudioProcessorEditor(*this);
  }

  bool hasEditor() const override { return true; }

  ~PluginProcessor();

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

 private:
  std::unique_ptr<juce::AudioProcessorGraph> audioGraph;
  juce::AudioProcessorGraph::Node::Ptr audioInputNode;            // access to hardware input
  juce::AudioProcessorGraph::Node::Ptr audioOutputNode;           // access to hardware output
  std::vector<juce::AudioProcessorGraph::Node::Ptr> dilateNodes;  // Spectral Dilation Processor
  juce::AudioProcessorPlayer
    processorPlayer;  // this DSP chain will be executed by the processorPlayer

  juce::AudioParameterChoice* fftOrderMenu;
  juce::AudioParameterFloat* focalPointSlider;
  juce::AudioParameterFloat* dilationFactorSlider;

  juce::SmoothedValue<float> focalPoint;
  juce::SmoothedValue<float> dilationFactor;

  float focalBin;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
#endif