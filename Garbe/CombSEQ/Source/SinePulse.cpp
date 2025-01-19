/*
  ==============================================================================

    SinePulse.cpp
    Created: 3 Dec 2024 3:32:58pm
    Author:  ngarb

  ==============================================================================
*/

#include "SinePulse.h"
#include <algorithm>

float SinePulse::gen() 
{
    auto currentSample = (float)std::sin(currentAngle) * currentPulseLevel();
    currentAngle += angleDelta;
    currentAngle = std::fmod(currentAngle, 2.0f * juce::MathConstants<float>::pi);


    pulseCounter++;
    if (pulseCounter >= (pulseIntervalMs / 1000) * sr) {
        pulseCounter = 0;
    }

    return currentSample;
}

void SinePulse::prepareToPlay(double sampleRate, int samplesPerBlock) 
{
    sr = sampleRate;
    audioBuffer = std::make_unique<juce::AudioBuffer<float>>(1, static_cast<int>(sr));

    updateAngleData(500.f);
}

void SinePulse::updateAngleData(float newFreq) {
    freq = newFreq;

    auto cyclesPerSample = newFreq / sr;
    angleDelta = cyclesPerSample * 2.0f * juce::MathConstants<float>::pi;
}

float SinePulse::currentPulseLevel() {
    float pulseDuration = 8000.f;
    float levelDelta = 1.f / pulseDuration;

    return std::clamp(1.f - (levelDelta * pulseCounter), 0.f, 1.f);
}