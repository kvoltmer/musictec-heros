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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CombSEQAudioProcessorEditor)
};
