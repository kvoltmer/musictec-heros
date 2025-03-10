/*
  ==============================================================================

    CombFilterProcessor.h
    Created: 22 Jan 2025 3:04:19pm
    Author:  ngarb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DelayMonoProcessor.h"

class CombFilterProcessor
{
public:
    CombFilterProcessor();
    ~CombFilterProcessor() = default;

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void prepareToPlay(double sampleRate, int samplesPerBlock);

    void setFrequency(float newFrequency);

private:
    double _sampleRate = 44100.0;
    int _bufferSize = 512;

    std::unique_ptr<DelayMonoProcessor> _delayProcessor;
};