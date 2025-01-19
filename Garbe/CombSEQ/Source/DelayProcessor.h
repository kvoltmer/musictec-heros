/*
  ==============================================================================

    DelayProcessor.h
    Created: 3 Dec 2024 2:43:28pm
    Author:  ngarb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DelayProcessor
{
public:
	DelayProcessor();
	~DelayProcessor() = default;

    float process(float in);
    void prepareToPlay(double sampleRate, int samplesPerBlock);

private:
    std::unique_ptr <juce::AudioBuffer<float>> audioBuffer;
    int sampleCounter = 0;

    double sr = 44100.f;
};

