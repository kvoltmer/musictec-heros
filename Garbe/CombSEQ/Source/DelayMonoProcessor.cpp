/*
  ==============================================================================

    DelayMonoProcessor.cpp
    Created: 3 Dec 2024 2:43:28pm
    Author:  ngarb

  ==============================================================================
*/


#include <iostream>

#include "DelayMonoProcessor.h"

inline float cubicInterp(float x, float y0, float y1, float y2, float y3)
{
    // 4-point, 3rd-order Hermite (x-form)
    const float c0 = y1;
    const float c1 = 0.5f * (y2 - y0);
    const float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
    const float c2 = y0 - y1 + c1 - c3;

    return ((c3 * x + c2) * x + c1) * x + c0;
}


DelayMonoProcessor::DelayMonoProcessor()
{
}

float DelayMonoProcessor::process(float in) {
    auto bufferSize = _delayLine->getNumSamples();
    auto buffPointer = _delayLine->getWritePointer(0);
    buffPointer[_writePosition] = in;
    
    // Linear smoothing
    //_delay += _smoothingRate * (_targetDelay - _delay);
    _delay = _targetDelay;

    float readPosition = static_cast<float>(_writePosition) + bufferSize - _delay;

    int readPosInt = static_cast<int>(readPosition);
    float frac = readPosition - readPosInt;

    int i0 = (readPosInt - 2 + bufferSize) % bufferSize;
    int i1 = (readPosInt - 1 + bufferSize) % bufferSize;
    int i2 = readPosInt % bufferSize;
    int i3 = (readPosInt + 1 + bufferSize) % bufferSize;

    float y0 = buffPointer[i0];
    float y1 = buffPointer[i1];
    float y2 = buffPointer[i2];
    float y3 = buffPointer[i3];

    // Perform cubic interpolation
    float delayedValue = cubicInterp(frac, y0, y1, y2, y3);

    _writePosition++;

    if (_writePosition >= bufferSize) {
        _writePosition -= bufferSize;
    }

    return delayedValue;
}

void DelayMonoProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    _sampleRate = sampleRate;
    _delayLine = std::make_unique<juce::AudioBuffer<float>>(1, static_cast<int> (_sampleRate * (_maxDelayMs / 1000)));

    _delayLine->clear();

}

void DelayMonoProcessor::setDelay(float newDelayMs) {
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


    _targetDelay = newDelayMs * 0.001 * _sampleRate;
}