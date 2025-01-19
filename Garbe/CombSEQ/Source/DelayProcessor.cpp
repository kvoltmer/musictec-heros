/*
  ==============================================================================

    DelayProcessor.cpp
    Created: 3 Dec 2024 2:43:28pm
    Author:  ngarb

  ==============================================================================
*/

// TODO: Stereo processing?

#include <iostream>

#include "DelayProcessor.h"

inline float cubicInterp(float x, float y0, float y1, float y2, float y3)
{
    // 4-point, 3rd-order Hermite (x-form)
    const float c0 = y1;
    const float c1 = 0.5f * (y2 - y0);
    const float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
    const float c2 = y0 - y1 + c1 - c3;

    return ((c3 * x + c2) * x + c1) * x + c0;
}


DelayProcessor::DelayProcessor()
{
}

float DelayProcessor::process(float in) {
    auto bufferSize = _delayLine->getNumSamples();
    auto buffPointer = _delayLine->getWritePointer(0);
    buffPointer[_sampleCounter] = in;

    if (_sampleCounter >= bufferSize) {
        _sampleCounter = 0;
    }
    
    // Linear smoothing
    _delay += _smoothingRate * (_targetDelay - _delay);
    //_delay = _targetDelay;

    //int readPos = (_sampleCounter + bufferSize - static_cast<int>(_delay)) % bufferSize;
    //float delayedValue = buffPointer[readPos];

    float readPosition = _sampleCounter + bufferSize - _delay;
    if (readPosition >= bufferSize) {
        readPosition -= bufferSize;
    }

    int readPosInt = static_cast<int>(readPosition);
    float frac = readPosition - readPosInt;

    float y0 = buffPointer[(readPosInt - 1 + bufferSize) % bufferSize];
    float y1 = buffPointer[readPosInt];
    float y2 = buffPointer[(readPosInt + 1) % bufferSize];
    float y3 = buffPointer[(readPosInt + 2) % bufferSize];

    // Perform cubic interpolation
    float delayedValue = cubicInterp(frac, y0, y1, y2, y3);

    _sampleCounter++;

    return delayedValue;
}

void DelayProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    _sampleRate = sampleRate;
    _delayLine = std::make_unique<juce::AudioBuffer<float>>(1, static_cast<int> (_sampleRate * (_maxDelayMs / 1000)));

    _delayLine->clear();

}

void DelayProcessor::setDelay(float newDelayMs) {
    if (newDelayMs < 0) {
        std::cerr << "Tried to set negative delay value: " << newDelayMs << std::endl;
        _delay = 0;
        return;
    }

    if (newDelayMs > _maxDelayMs) {
        std::cerr << "New delay time larger than maximum delay time of proccesor:: " << newDelayMs << std::endl;
        _delay = 0;
        return;
    }


    _targetDelay = (newDelayMs / 1000) * _sampleRate;
}