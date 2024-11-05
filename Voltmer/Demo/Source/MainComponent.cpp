#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    
    addAndMakeVisible (frequencySlider1);
    frequencySlider1.setRange (5.0, 5000.0);
    frequencySlider1.setSkewFactorFromMidPoint (500.0); // [4]
    frequencySlider1.onValueChange = [this] {
                if (currentSampleRate > 0.0)
                    updateAngleDelta();
            };

    
    addAndMakeVisible (frequencySlider2);
    frequencySlider2.setRange (5.0, 5000.0);
    frequencySlider2.setSkewFactorFromMidPoint (500.0); // [4]
    frequencySlider2.onValueChange = [this] {
                if (currentSampleRate > 0.0)
                    updateAngleDelta();
            };
    
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    currentSampleRate = sampleRate;
    updateAngleDelta();
}

float MainComponent::generateNoise(float input)
{
    auto value = random.nextFloat() - 0.5f;
    return value;
}

float MainComponent::generateSine(float input)
{
    auto sineValue = (float) std::sin (currentAngle[0]);
    currentAngle[0] += angleDelta[0];

    return sineValue;
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& info)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    
    info.clearActiveBufferRegion();
    
    // https://docs.juce.com/master/tutorial_simple_synth_noise.html
    
    
    auto buffer = info.buffer->getWritePointer(0, info.startSample);
    
    
    for(auto i = 0; i < info.numSamples; i++) {
        //buffer[i] = generateNoise(buffer[i]);
        
        buffer[i] = generateSine(buffer[i]) * 100.0;
        
        if (buffer[i] > 1.0)
            buffer[i] = 1.0;
        
        if (buffer[i] < -1.0)
            buffer[i] = -1.0;
        //buffer[i] = std::tanh(buffer[i]);

    }
    
    
    
    
    // get the pointer to the left channel
 //   auto leftChannel = info.buffer->getWritePointer(0, info.startSample);
    
    
//    for (auto i = 0; i < info.numSamples; i++) {
//        float value = random.nextFloat() - 0.5f;
//        leftChannel[i] = value;
//    }
    
    
//    for (auto i = 0; i < info.numSamples; ++i) {
//        auto randomValue = (random.nextFloat() - 0.5f) * 0.125;
//        auto sineValue = (float) std::sin (currentAngle[0]);
//        leftChannel[i] = randomValue * sineValue * 0.5f;
//        currentAngle[0] += angleDelta[0];
//    }
    
    
//
//    for (auto i = 0; i < info.numSamples; ++i) {
//        //auto randomValue = (random.nextFloat() - 0.5f) * 0.125;
//        auto sineValue1 = (float) std::sin (currentAngle[0]);
//        auto sineValue2 = (float) std::sin (currentAngle[1]);
//
//        leftChannel[i] = sineValue1 * sineValue2 * index;
//        leftChannel[i] *= 0.5;
//
//        currentAngle[0] += angleDelta[0];
//        currentAngle[1] += angleDelta[1];
//
//        if (currentAngle[0] > 1.0)
//            currentAngle[0] -= 1.0;
//
//        //std::cout << currentAngle[0] << std::endl;
//    }
//
//    auto rightChannel = info.buffer->getWritePointer(1, info.startSample);
//    memcpy(rightChannel, leftChannel, info.numSamples * sizeof(float));
//
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}



//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    frequencySlider1.setBounds(0, 0, 500, 30);
    frequencySlider2.setBounds(0, 50, 500, 30);
}
