/*
  ==============================================================================

    DelayMultiProcessor.cpp
    Created: 21 Jan 2025 4:05:37pm
    Author:  ngarb

  ==============================================================================
*/

#include <iostream>

#include "DelayMultiProcessor.h"


DelayMultiProcessor::DelayMultiProcessor(size_t channels)
{
    if (channels < 1) {
        std::cerr << "Passed channel-count too low: " << channels << std::endl;
        channels = 1;
    }
    _nDesiredChannels = channels;
}

void DelayMultiProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    _sampleRate = sampleRate;
    setChannelCount(_nDesiredChannels);

    for (auto& delay : _delayChannels) {
        delay->prepareToPlay(sampleRate, samplesPerBlock);
    }

}

void DelayMultiProcessor::setDelay(int channel, float newDelayMs) {
    _delayChannels[channel]->setDelay(newDelayMs);
}

void DelayMultiProcessor::setChannelCount(unsigned int channelCount) {
    _delayChannels.clear();

    for (int i = 0; i < channelCount; i++) {
        _delayChannels.push_back(std::make_unique<DelayMonoProcessor>());
    }
}

void DelayMultiProcessor::processBlock( juce::AudioBuffer<float>& buffer,
                                        juce::MidiBuffer& midiMessages) {
    for (auto channel = 0; channel < buffer.getNumChannels(); channel++) {

        for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
            auto input = buffer.getReadPointer(channel)[sample];
            auto output = _delayChannels[channel]->process(input);

            buffer.getWritePointer(channel)[sample] = output;
        }
    }
}