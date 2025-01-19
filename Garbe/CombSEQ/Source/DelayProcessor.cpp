/*
  ==============================================================================

    DelayProcessor.cpp
    Created: 3 Dec 2024 2:43:28pm
    Author:  ngarb

  ==============================================================================
*/

#include "DelayProcessor.h"


DelayProcessor::DelayProcessor()
{
}

float DelayProcessor::process(float in) {
    auto bufferSize = audioBuffer->getNumSamples();
    auto buffPointer = audioBuffer->getWritePointer(0);
    buffPointer[sampleCounter] = in;

    if (sampleCounter >= bufferSize) {
        sampleCounter = 0;
    }
    
    int readPos = (sampleCounter + bufferSize - 22050) % bufferSize;
    float delayedValue = buffPointer[readPos];

    sampleCounter++;

    return delayedValue;
}

void DelayProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;
    audioBuffer = std::make_unique<juce::AudioBuffer<float>>(1, static_cast<int> (sr));

    audioBuffer->clear();

}