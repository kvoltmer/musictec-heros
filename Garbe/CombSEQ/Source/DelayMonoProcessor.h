/*
  ==============================================================================

    DelayMonoProcessor.h
    Created: 3 Dec 2024 2:43:28pm
    Author:  ngarb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DelayMonoProcessor
{
public:
	DelayMonoProcessor();
	~DelayMonoProcessor() = default;

    float process(float in);
    void prepareToPlay(double sampleRate, int samplesPerBlock);

    void setDelay(float delay);
    float getDelay() { return _delay; };

private:
    std::unique_ptr <juce::AudioBuffer<float>> _delayLine;

    int _writePosition = 0;
    float _delay = 0;
    float _maxDelayMs = 2000.0f;

    // TODO: Fix initial set
    float _smoothingRate = 0.01f;
    float _targetDelay = 0.f;

    double _sampleRate = 44100.f;
};

