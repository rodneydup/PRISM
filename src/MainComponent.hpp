#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module
// headers directly. If you need to remain compatible with Projucer-generated
// builds, and have called `juce_generate_juce_header(<thisTarget>)` in your
// CMakeLists.txt, you could `#include <JuceHeader.h>` here instead, to make all
// your module headers visible.
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_extra/juce_gui_extra.h>

// our components
#include "Processors/AudioSettingsComponent.hpp"
#include "Processors/DilateProcessor.hpp"
#include "Processors/SpectrogramComponent.hpp"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component, private juce::Timer {
 public:
  //==============================================================================
  MainComponent();
  ~MainComponent();

  // Graphics
  // stuff==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

  void timerCallback() override;

 private:
  // Instead of inheriting from juce::AudioAppComponent, we make our own deviceManager
  juce::AudioDeviceManager deviceManager;
  // The audio graph will store and set the full DSP chain.
  std::unique_ptr<juce::AudioProcessorGraph> audioGraph;
  // Nodes
  juce::AudioProcessorGraph::Node::Ptr audioInputNode;            // access to hardware input
  juce::AudioProcessorGraph::Node::Ptr audioOutputNode;           // access to hardware output
  juce::AudioProcessorGraph::Node::Ptr testToneNode;              // Sine tone
  juce::AudioProcessorGraph::Node::Ptr dilateNode;                // Spectral Dilate
  std::unique_ptr<juce::AudioProcessorEditor> dilateEditor;       // Spectral Dilate Editor
  juce::AudioProcessorGraph::Node::Ptr spectrogramNode;           // Spectrogram
  std::unique_ptr<juce::AudioProcessorEditor> spectrogramEditor;  // Spectrogram editor

  // this DSP chain will be executed by the processorPlayer
  juce::AudioProcessorPlayer processorPlayer;
  // Audio Settings window for changing IO settings at runtime
  audioSettingsComponent audioSettings;

  // this is JUCE's convenience MACRO to make sure we don't make terrible, terrible mistakes with
  // our pointers
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};