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
    

    addAndMakeVisible(delaySlider);
    delaySlider.setRange(0.0f, 500.0f);
    delaySlider.setTextValueSuffix(" Ms");
    delayAttachement.reset(new SliderAttachment(state, "delay", delaySlider));
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 160, delaySlider.getTextBoxHeight());
    delaySlider.setValue(0.0f);

    addAndMakeVisible(delayLabel);
    delayLabel.setText("Delay", juce::dontSendNotification);
    delayLabel.attachToComponent(&delaySlider, true);


    addAndMakeVisible(combFreqSlider);
    combFreqSlider.setRange(0.0f, 20000.0f);
    combFreqSlider.setTextValueSuffix(" Hz");
    combFreqAttachement.reset(new SliderAttachment(state, "combFreq", combFreqSlider));
    combFreqSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 160, combFreqSlider.getTextBoxHeight());
    combFreqSlider.setValue(500.0f);

    addAndMakeVisible(combFreqLabel);
    combFreqLabel.setText("Comb Frequency", juce::dontSendNotification);
    combFreqLabel.attachToComponent(&combFreqSlider, true);

    /*
    for (size_t i = 0; i < 10; i++) {
        juce::Slider* newSlider = new juce::Slider();
        newSlider->setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);

        addAndMakeVisible( sliders.add(newSlider) );
    }
    */

    setSize (800, 300);
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
    std::cout << "paint" << std::endl;
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}
#
void CombSEQAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    juce::FlexBox noiseBox;
    noiseBox.flexWrap = juce::FlexBox::Wrap::wrap;
    noiseBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    noiseBox.items.add(juce::FlexItem(noiseLevelSlider).withMinHeight(50.0f).withMinWidth(300.0f).withFlex(1));
    noiseBox.items.add(juce::FlexItem(delaySlider).withMinHeight(50.0f).withMinWidth(300.0f).withFlex(1));
    noiseBox.items.add(juce::FlexItem(combFreqSlider).withMinHeight(50.0f).withMinWidth(300.0f).withFlex(1));
    bounds = bounds.removeFromBottom(bounds.getHeight() / 2);
    noiseBox.performLayout(bounds);

    /*  juce::FlexBox sliderBox;
    sliderBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    sliderBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    for (auto s : sliders) {
        sliderBox.items.add(juce::FlexItem(*s).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));
    }

    sliderBox.performLayout(bounds);

    */
}
