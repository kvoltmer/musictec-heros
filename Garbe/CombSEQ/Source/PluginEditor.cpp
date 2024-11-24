/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CombSEQAudioProcessorEditor::CombSEQAudioProcessorEditor(juce::AudioProcessor& parent, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(parent),
    state(vts)
{
    addAndMakeVisible(noiseLevelSlider);
    noiseLevelSlider.setRange(0.0f, 1.0f);
    noiseLevelSlider.setTextValueSuffix(" Db");
    noiseLevelAttachement.reset(new SliderAttachment(state, "noiseLevel", noiseLevelSlider));
    noiseLevelSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 160, noiseLevelSlider.getTextBoxHeight());
    noiseLevelSlider.setValue(1.0f);

    addAndMakeVisible(noiseLevelLabel);
    noiseLevelLabel.setText("Level", juce::dontSendNotification);
    noiseLevelLabel.attachToComponent(&noiseLevelSlider, true);


    setSize (400, 300);
}

CombSEQAudioProcessorEditor::~CombSEQAudioProcessorEditor()
{
}

//==============================================================================
void CombSEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void CombSEQAudioProcessorEditor::resized()
{
    auto sliderLeft = 120;
    noiseLevelSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
}
