/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class CombSEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    CombSEQAudioProcessorEditor(juce::AudioProcessor& parent, juce::AudioProcessorValueTreeState& vts);
    ~CombSEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    juce::AudioProcessorValueTreeState& state;

    juce::Slider noiseLevelSlider;
    juce::Label noiseLevelLabel;
    std::unique_ptr<SliderAttachment> noiseLevelAttachement;

    juce::Slider delaySlider;
    juce::Label delayLabel;
    std::unique_ptr<SliderAttachment> delayAttachement;

    juce::Slider combFreqSlider;
    juce::Label combFreqLabel;
    std::unique_ptr<SliderAttachment> combFreqAttachement;

    juce::OwnedArray<juce::Slider> sliders;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CombSEQAudioProcessorEditor)
};
