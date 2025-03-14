/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginEditor.h"
#include "SinePulse.h"
#include "DelayMonoProcessor.h"
#include "CombFilterProcessor.h"


//==============================================================================
/**
*/
class CombSEQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    CombSEQAudioProcessor();
    ~CombSEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override { return new CombSEQAudioProcessorEditor(*this, state); }
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


private:
    juce::Random random;

    juce::AudioProcessorValueTreeState state;

    float previousNoiseLevel;
    std::atomic<float>* noiseLevelParameter = nullptr;

    std::atomic<float>* delayParameter = nullptr;

    std::atomic<float>* combFreqParameter = nullptr;

    std::unique_ptr<SinePulse> sinePulse;
    std::unique_ptr<DelayMonoProcessor> delayProcessor;
    std::unique_ptr<CombFilterProcessor> combProcessor;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CombSEQAudioProcessor)
};
