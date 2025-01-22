/*
  ==============================================================================

    CombFilterProcessor.cpp
    Created: 22 Jan 2025 3:04:19pm
    Author:  ngarb

  ==============================================================================
*/

#include "CombFilterProcessor.h"

#include <iostream>


CombFilterProcessor::CombFilterProcessor()
{
}

void CombFilterProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    _sampleRate = sampleRate;

    _delayProcessor = std::make_unique<DelayMonoProcessor>();
    _delayProcessor->prepareToPlay(sampleRate, samplesPerBlock);
}

void CombFilterProcessor::setFrequency(float newFrequency) {
    _delayProcessor->setDelay(1 / newFrequency * 1000);
}

void CombFilterProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
        auto input = buffer.getReadPointer(0)[sample];
        auto delayedSample = _delayProcessor->process(input);

        buffer.getWritePointer(0)[sample] = input - delayedSample;
    }
}
