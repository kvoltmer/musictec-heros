/*
  ==============================================================================

    SinePulse.h
    Created: 3 Dec 2024 3:32:58pm
    Author:  ngarb

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SinePulse
{
public:
    SinePulse(float freq, float intervalMs) :
        freq(freq),
        pulseIntervalMs(intervalMs)
    {
    }
	~SinePulse() = default;

    float gen();
    void prepareToPlay(double sampleRate, int samplesPerBlock);

    void updateAngleData(float newFreq);
    float currentPulseLevel();


private:
    std::unique_ptr<juce::AudioBuffer<float>> audioBuffer;
    double sr = 44100.f;

    float currentAngle = 0.0, angleDelta = 0.0;
    float freq = 500.f;

    float pulseIntervalMs = 50; // ms
    float pulseCounter = 0;

};