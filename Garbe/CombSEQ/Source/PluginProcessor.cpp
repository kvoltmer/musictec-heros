/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


// Parameters
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    auto paramNoiseLevel = std::make_unique<juce::AudioParameterFloat>( "noiseLevel",           // parameterID
                                                                        "NoiseLevel",           // parameter name
                                                                        0.0f,                   // minimum value
                                                                        1.0f,                   // maximum value
                                                                        1.0f);                  // default value

    params.push_back(std::move(paramNoiseLevel));

    auto paramDelay = std::make_unique<juce::AudioParameterFloat>(      "delay",                // parameterID
                                                                        "Delay",                // parameter name
                                                                        0.0f,                   // minimum value
                                                                        500.0f,                 // maximum value
                                                                        0.0f);                  // default value

    params.push_back(std::move(paramDelay));

    auto paramCombFreq = std::make_unique<juce::AudioParameterFloat>(   "combFreq",             // parameterID
                                                                        "CombFreq",             // parameter name
                                                                        0.0f,                   // minimum value
                                                                        20000.0f,                 // maximum value
                                                                        100.0f);                  // default value

    params.push_back(std::move(paramCombFreq));

    return { params.begin(), params.end() };
}


//==============================================================================
CombSEQAudioProcessor::CombSEQAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
state(*this, nullptr, juce::Identifier("PARAMS"), createParameterLayout())
{
    noiseLevelParameter = state.getRawParameterValue("noiseLevel");
    delayParameter = state.getRawParameterValue("delay");
    combFreqParameter = state.getRawParameterValue("combFreq");

    sinePulse = std::make_unique<SinePulse> (500.f, 2000) ;
    delayProcessor = std::make_unique<DelayMonoProcessor> ();
    combProcessor = std::make_unique<CombFilterProcessor> ();

    auto channelCount = getTotalNumOutputChannels();


}

CombSEQAudioProcessor::~CombSEQAudioProcessor()
{
}

//==============================================================================
const juce::String CombSEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CombSEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CombSEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CombSEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CombSEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CombSEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CombSEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CombSEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CombSEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void CombSEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CombSEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    previousNoiseLevel = *noiseLevelParameter;

    sinePulse->prepareToPlay(sampleRate, samplesPerBlock);

    delayProcessor->prepareToPlay(sampleRate, samplesPerBlock);
    delayProcessor->setDelay(*delayParameter);

    combProcessor->prepareToPlay(sampleRate, samplesPerBlock);
    combProcessor->setFrequency(500);

}

void CombSEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CombSEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif
    
    return true;
  #endif
}
#endif

void CombSEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto& level = *noiseLevelParameter;
    auto levelScale = level * 2.0f;

    // TODO: Listener?
    delayProcessor->setDelay(*delayParameter);
    combProcessor->setFrequency(*combFreqParameter);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (size_t sample = 0; sample < buffer.getNumSamples(); sample++) {
            channelData[sample] = sinePulse->gen();
        }

        
        /*for (size_t sample = 0; sample < buffer.getNumSamples(); sample++) {
            channelData[sample] = random.nextFloat() * levelScale - level;
        }*/

        combProcessor->processBlock(buffer, midiMessages);
    }
}

//==============================================================================
bool CombSEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}
/*
juce::AudioProcessorEditor* CombSEQAudioProcessor::createEditor()
{
    return new CombSEQAudioProcessorEditor (*this, state);
}*/

//==============================================================================
void CombSEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto editor = state.copyState();
    std::unique_ptr<juce::XmlElement> xml(editor.createXml());
    copyXmlToBinary(*xml, destData);
}

void CombSEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(state.state.getType()))
            state.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CombSEQAudioProcessor();
}
