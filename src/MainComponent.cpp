#include "MainComponent.hpp"

//==============================================================================
MainComponent::MainComponent()
  : audioGraph(std::make_unique<juce::AudioProcessorGraph>()), audioSettings(deviceManager) {
  setSize(800, 500);
  startTimerHz(60);

  // tell the ProcessorPlayer what audio callback function to play (.get() needed since audioGraph
  // is a unique_ptr)
  processorPlayer.setProcessor(audioGraph.get());

  // simplest way to start audio device. Uses whichever device the current system (mac/pc/linux
  // machine) uses
  deviceManager.initialiseWithDefaultDevices(2, 2);

  // Set to Jack by default
  // deviceManager.setCurrentAudioDeviceType("JACK", true);

  // Tell the processor player to keep moving every time the device requests more data
  deviceManager.addAudioCallback(&processorPlayer);
  // set up the graph
  audioGraph->clear();  // likely not needed but won't hurt
  // a node that passes in input from your device
  audioInputNode =
    audioGraph->addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
      juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
  // a node that passes audio out to your device
  audioOutputNode =
    audioGraph->addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
      juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));

  // a simple sine tone generator node
  testToneNode = audioGraph->addNode(std::make_unique<ToneGenerator>());
  // set the details of the processor (io and samplerate/buffersize
  testToneNode->getProcessor()->setPlayConfigDetails(
    0, 2, deviceManager.getAudioDeviceSetup().sampleRate,
    deviceManager.getAudioDeviceSetup().bufferSize);
  // connect the 'left' channel
  // audioGraph->addConnection({{testToneNode->nodeID, 0}, {audioOutputNode->nodeID, 0}});
  // connect the 'right' channel
  // audioGraph->addConnection({{testToneNode->nodeID, 1}, {audioOutputNode->nodeID, 1}});

  // Spectrogram
  SpectrogramNode = audioGraph->addNode(std::make_unique<SpectrogramComponent>());

  SpectrogramNode->getProcessor()->setPlayConfigDetails(
    1, 0, deviceManager.getAudioDeviceSetup().sampleRate,
    deviceManager.getAudioDeviceSetup().bufferSize);
  audioGraph->addConnection({{audioInputNode->nodeID, 0}, {SpectrogramNode->nodeID, 0}});
  // audioGraph->addConnection({{audioInputNode->nodeID, 1}, {SpectrogramNode->nodeID, 1}});
  auto* spectrogram = SpectrogramNode->getProcessor()->createEditor();
  // spectrogram->setVisible(true);

  // audioGraph->addConnection(
  //   {{audioInputNode->nodeID, 0}, {audioOutputNode->nodeID, 0}});  // FEEDBACK CAREFUL
  // audioGraph->addConnection(
  //   {{audioInputNode->nodeID, 1}, {audioOutputNode->nodeID, 1}});  // FEEDBACK CAREFUL

  // DilateNode = audioGraph->addNode(std::make_unique<DilateComponent>());
  // DilateNode->getProcessor()->setPlayConfigDetails(
  //   1, 1, deviceManager.getAudioDeviceSetup().sampleRate,
  //   deviceManager.getAudioDeviceSetup().bufferSize);
  // audioGraph->addConnection({{audioInputNode->nodeID, 0}, {DilateNode->nodeID, 0}});
  // audioGraph->addConnection({{DilateNode->nodeID, 0}, {audioOutputNode->nodeID, 0}});

  audioSettings.button.setBounds(getLocalBounds().removeFromTop(50));
  addAndMakeVisible(audioSettings.button);
  addAndMakeVisible(spectrogram);
}

void MainComponent::timerCallback() { repaint(); }

//==============================================================================
void MainComponent::paint(juce::Graphics& g) {}

void MainComponent::resized() {
  // This is called when the MainComponent is resized.
  // If you add any child components, this is where you should
  // update their positions.
  audioSettings.button.setBounds(getLocalBounds().removeFromTop(50));
}

MainComponent::~MainComponent() {
  deviceManager.closeAudioDevice();
  // unfortunately the naming convention to de-allocate a unique pointer is .reset()
  audioGraph.reset();
}