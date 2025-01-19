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
    float maxLevel = 0.25f;
    auto level = maxLevel * currentPulseLevel();

    auto currentSample = (float)std::sin(currentAngle) * level;
    currentAngle += angleDelta;

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
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
}

float SinePulse::currentPulseLevel() {
    float pulseDuration = 8000.f;
    float levelDelta = 1.f / pulseDuration;

    return std::clamp(1.f - (levelDelta * pulseCounter), 0.f, 1.f);
}