#include "DilateComponent.hpp"

DilateComponent::DilateComponent() : forwardFFT(fftOrder) {}

void DilateComponent::prepareToPlay(double, int) {}
void DilateComponent::releaseResources() {}

void DilateComponent::processBlock(juce::AudioBuffer<float>& audioBuffer,
                                   juce::MidiBuffer& midiBuffer) {
  if (audioBuffer.getNumChannels() > 0) {
    float* channelData = audioBuffer.getWritePointer(0, 0);
    forwardFFT.performFrequencyOnlyForwardTransform(channelData);
  }
}