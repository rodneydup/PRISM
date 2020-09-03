#include "SpectrogramComponent.hpp"

SpectrogramComponent::SpectrogramComponent() : spectrogramImage(juce::Image::RGB, 512, 512, true) {
  forwardFFT = std::make_unique<juce::dsp::FFT>(fftOrder);
  fifo.resize(fftSize);
  fftData.resize(fftSize * 2);
}

SpectrogramComponent::~SpectrogramComponent() {}

//==============================================================================
void SpectrogramComponent::prepareToPlay(double, int) {}
void SpectrogramComponent::releaseResources() {}

void SpectrogramComponent::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                        juce::MidiBuffer& midiBuffer) {
  if (!bypass) {
    if (audioBuffer.getNumChannels() > 0) {
      float* channelData = audioBuffer.getWritePointer(0, 0);
      for (auto i = 0; i < audioBuffer.getNumSamples(); ++i) pushNextSampleIntoFifo(channelData[i]);
    }
  }
}

//==============================================================================

void SpectrogramComponent::pushNextSampleIntoFifo(float sample) noexcept {
  // if the fifo contains enough data, set a flag to say
  // that the next line should now be rendered..
  if (fifoIndex == fftSize) {
    std::fill(fftData.begin(), fftData.end(), 0.0f);
    std::copy(fifo.begin(), fifo.end(), fftData.begin());
    drawNextLineOfSpectrogram();

    fifoIndex = 0;
  }

  fifo[(size_t)fifoIndex++] = sample;
}

void SpectrogramComponent::drawNextLineOfSpectrogram() {
  auto rightHandEdge = spectrogramImage.getWidth() - 1;
  auto imageHeight = spectrogramImage.getHeight();

  // first, shuffle our image leftwards by 1 pixel..
  spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);  // [1]

  // then render our FFT data..
  forwardFFT->performFrequencyOnlyForwardTransform(fftData.data());  // [2]

  // find the range of values produced, so we can scale our rendering to
  // show up the detail clearly
  auto maxLevel = juce::FloatVectorOperations::findMinAndMax(fftData.data(), fftSize / 2);  // [3]

  for (auto y = 1; y < imageHeight; ++y)  // [4]
  {
    auto skewedProportionY = 1.0f - std::exp(std::log((float)y / (float)imageHeight) * 0.2f);
    auto fftDataIndex =
      (size_t)juce::jlimit(0, fftSize / 2, (int)(skewedProportionY * fftSize / 2));
    auto level =
      juce::jmap(fftData[fftDataIndex], 0.0f, juce::jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

    spectrogramImage.setPixelAt(rightHandEdge, y,
                                juce::Colour::fromHSV(level, 1.0f, level, 1.0f));  // [5]
  }
}

void SpectrogramComponent::changeOrder(int order) {
  bypass = true;
  int fftOrder = order;
  int fftSize = 1 << fftOrder;
  forwardFFT = std::make_unique<juce::dsp::FFT>(fftOrder);
  fifo.resize(fftSize);
  fftData.resize(fftSize * 2);
  bypass = false;
}

//==================================================================================================

SpectrogramComponent::Editor::Editor(SpectrogramComponent& owner) : AudioProcessorEditor(owner) {
  spectrogramImage = &owner.spectrogramImage;
  setOpaque(true);
  setSize(700, 500);
  for (int i = 6; i < 13; i++) fftOrderMenu.addItem(std::to_string(int(pow(2, i))), i);
  fftOrderMenu.setSelectedId(10);
  addAndMakeVisible(fftOrderMenu);
  fftOrderMenu.onChange = [&] { owner.changeOrder(fftOrderMenu.getSelectedId()); };
}

SpectrogramComponent::Editor::~Editor() {}

void SpectrogramComponent::Editor::paint(juce::Graphics& g) {
  g.fillAll(juce::Colours::black);
  g.setOpacity(1.0f);
  auto rect = getParentComponent()->getLocalBounds().reduced(100);
  if (!rect.isEmpty()) setBounds(rect);
  fftOrderMenu.setBounds(getLocalBounds().removeFromTop(25));
  g.drawImage(*spectrogramImage, getLocalBounds().toFloat());
}
