#include "MainComponent.hpp"

//==============================================================================
MainComponent::MainComponent()
  : audioGraph(std::make_unique<juce::AudioProcessorGraph>()), audioSettings(deviceManager) {
  setSize(800, 500);
  startTimerHz(30);

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
  spectrogramNode = audioGraph->addNode(std::make_unique<SpectrogramComponent>());

  spectrogramNode->getProcessor()->setPlayConfigDetails(
    1, 0, deviceManager.getAudioDeviceSetup().sampleRate,
    deviceManager.getAudioDeviceSetup().bufferSize);
  // audioGraph->addConnection({{audioInputNode->nodeID, 0}, {spectrogramNode->nodeID, 0}});
  // This is the best way I've found to get the editor and be able to display it. Just have your
  // mainComponent own a pointer to an editor, then point it to the editor when it's created.
  spectrogramEditor = spectrogramNode->getProcessor()->createEditor();

  // audioGraph->addConnection(
  //   {{audioInputNode->nodeID, 0}, {audioOutputNode->nodeID, 0}});  // FEEDBACK CAREFUL
  // audioGraph->addConnection(
  //   {{audioInputNode->nodeID, 1}, {audioOutputNode->nodeID, 1}});  // FEEDBACK CAREFUL

  dilateNode = audioGraph->addNode(std::make_unique<DilateComponent>());
  dilateNode->getProcessor()->setPlayConfigDetails(1, 1,
                                                   deviceManager.getAudioDeviceSetup().sampleRate,
                                                   deviceManager.getAudioDeviceSetup().bufferSize);
  audioGraph->addConnection(
    {{audioInputNode->nodeID, 0}, {dilateNode->nodeID, 0}});  // connect input to dilate
  audioGraph->addConnection(
    {{dilateNode->nodeID, 0}, {audioOutputNode->nodeID, 0}});  // connect dilate to output
  audioGraph->addConnection(
    {{dilateNode->nodeID, 0}, {spectrogramNode->nodeID, 0}});  // connect dilate to spectrogram
  dilateEditor = dilateNode->getProcessor()->createEditor();

  addAndMakeVisible(audioSettings.button);
  dilateEditor->setTopLeftPosition(10, 50);
  addAndMakeVisible(dilateEditor);
  addAndMakeVisible(spectrogramEditor);
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
  audioSettings.button.setBounds(getLocalBounds().removeFromTop(50));
}

MainComponent::~MainComponent() {
  deviceManager.closeAudioDevice();
  // unfortunately the naming convention to de-allocate a unique pointer is .reset()
  audioGraph.reset();
  // This is hamfisted, but it mitigates memory leaks in cases where the mainComponent is closed
  // before its children.
  deleteAllChildren();
}