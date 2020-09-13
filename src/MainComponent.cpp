#include "MainComponent.hpp"

//==============================================================================
MainComponent::MainComponent()
  : audioGraph(std::make_unique<juce::AudioProcessorGraph>()), audioSettings(deviceManager) {
  // tell the ProcessorPlayer what audio callback function to play (.get() needed since audioGraph
  // is a unique_ptr)
  processorPlayer.setProcessor(audioGraph.get());

  // simplest way to start audio device. Uses whichever device the current system (mac/pc/linux
  // machine) uses
  deviceManager.initialiseWithDefaultDevices(2, 2);

  // Set to Jack by default

  deviceManager.setCurrentAudioDeviceType("JACK", true);

  // Tell the processor player to keep moving every time the device requests more data
  deviceManager.addAudioCallback(&processorPlayer);
  // set up the graph
  audioGraph->clear();  // likely not needed but won't hurt

  // Add nodes to the graph
  // Input
  audioInputNode =
    audioGraph->addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
      juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
  // Output
  audioOutputNode =
    audioGraph->addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
      juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));

  // Dilate
  dilateNode = audioGraph->addNode(std::make_unique<DilateComponent>());
  dilateNode->getProcessor()->setPlayConfigDetails(1, 1,
                                                   deviceManager.getAudioDeviceSetup().sampleRate,
                                                   deviceManager.getAudioDeviceSetup().bufferSize);
  dilateEditor =
    std::unique_ptr<juce::AudioProcessorEditor>(dilateNode->getProcessor()->createEditor());

  // Spectrogram
  spectrogramNode = audioGraph->addNode(std::make_unique<SpectrogramComponent>());
  spectrogramNode->getProcessor()->setPlayConfigDetails(
    1, 0, deviceManager.getAudioDeviceSetup().sampleRate,
    deviceManager.getAudioDeviceSetup().bufferSize);
  spectrogramEditor =
    std::unique_ptr<juce::AudioProcessorEditor>(spectrogramNode->getProcessor()->createEditor());

  // Connections
  audioGraph->addConnection(
    {{audioInputNode->nodeID, 0}, {dilateNode->nodeID, 0}});  // connect input to dilate
  audioGraph->addConnection(
    {{dilateNode->nodeID, 0}, {audioOutputNode->nodeID, 0}});  // connect dilate to output
  audioGraph->addConnection(
    {{dilateNode->nodeID, 0}, {spectrogramNode->nodeID, 0}});  // connect dilate to spectrogram

  audioSettings.button->setBounds(getLocalBounds().removeFromTop(50));

  addAndMakeVisible(audioSettings.button.get());
  dilateEditor->setTopLeftPosition(10, 50);
  addAndMakeVisible(dilateEditor.get());
  addAndMakeVisible(spectrogramEditor.get());

  setSize(800, 500);
  startTimerHz(30);
}

void MainComponent::timerCallback() {
  // Should be able to set the timer and call repaint from within the processorEditor itself, but it
  // gives segfaults when I try, so I'm setting up the timer and repaint call from here in the
  // mainComponent because it seems to work.
  spectrogramEditor->repaint();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g) {}

void MainComponent::resized() {
  // This is called when the MainComponent is resized.
  // If you add any child components, this is where you should
  // update their positions.
  auto rect = getLocalBounds();

  audioSettings.button->setBounds(rect.removeFromTop(50));
}

MainComponent::~MainComponent() {
  deviceManager.closeAudioDevice();
  audioGraph.reset();
}