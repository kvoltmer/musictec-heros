#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void updateAngleDelta()
    {
        auto cyclesPerSample1 = frequencySlider1.getValue() / currentSampleRate;
        angleDelta[0] = cyclesPerSample1 * 2.0 * juce::MathConstants<double>::pi;
        
        auto cyclesPerSample2 = frequencySlider2.getValue() / currentSampleRate;
        angleDelta[1] = cyclesPerSample2 * 2.0 * juce::MathConstants<double>::pi;
    }
    
    float generateNoise(float input);
    
    float generateSine(float input);

private:
    //==============================================================================
    // Your private member variables go here...
    juce::Random random;

    float myMember = 0.5f;
    
    double currentSampleRate = 0.0;
    double currentAngle[2] = {0.0, 0.0};
    double angleDelta[2] = {0.0, 0.0};
    double index = 0.5;
    
    juce::Slider frequencySlider1;
    juce::Slider frequencySlider2;
    juce::Slider indexSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

