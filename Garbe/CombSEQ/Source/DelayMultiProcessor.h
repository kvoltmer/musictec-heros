/*
  ==============================================================================

    DelayMultiProcessor.h
    Created: 21 Jan 2025 4:05:37pm
    Author:  ngarb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayMonoProcessor.h"

class DelayMultiProcessor
{
public:
	DelayMultiProcessor(size_t channels);
	~DelayMultiProcessor() = default;

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void prepareToPlay(double sampleRate, int samplesPerBlock);

    void setChannelCount(unsigned int channelCOunt);

    void setDelay(int channel, float delay);
    float getDelay() { return _delay; };

private:
    std::vector<std::unique_ptr<DelayMonoProcessor>> _delayChannels;
    float _delay = 0.f;

    float _sampleRate = 44100.f;
    int _bufferSize = 512;
    int _nDesiredChannels = 2;
};
