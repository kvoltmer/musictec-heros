/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayMonoProcessor.h"
#include "DelayMultiProcessor.h"
#include "SinePulse.h"

using namespace juce;
//==============================================================================
int main (int argc, char* argv[])
{
    double samplerate = 44100.0;
    unsigned int buffersize = 512;

    String path = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName();

    File myTargetFile(path + File::getSeparatorString() + "out.wav");
    TemporaryFile tempFile(myTargetFile);

    std::unique_ptr<OutputStream> outStream(tempFile.getFile().createOutputStream());

    if (outStream != nullptr) {
        const StringPairArray metadata;
        WavAudioFormat wav;
        std::unique_ptr<AudioFormatWriter> writer(wav.createWriterFor(outStream.release(), samplerate,
            (unsigned int)2,
            24,
            metadata, 0));
        if (writer != nullptr) {

            AudioBuffer<float> monoBuffer(1, 512);
            monoBuffer.clear();

            AudioBuffer<float> stereoBuffer(2, 512);
            stereoBuffer.clear();


            SinePulse sin(440.f, 500);
            sin.prepareToPlay(samplerate, buffersize);

            DelayMultiProcessor delay = DelayMultiProcessor(2);
            delay.prepareToPlay(samplerate, buffersize);
            delay.setDelay(0, 10);
            delay.setDelay(1, 250);

            double seconds = 10;
            int sampleCount = seconds * samplerate;
            int iterations = sampleCount / buffersize;

            auto ptr = monoBuffer.getWritePointer(0);

            for (auto i = 0; i < iterations; i++) {
                monoBuffer.clear();

                for (auto s = 0; s < buffersize; s++){
                    ptr[s] = sin.gen();
                }

                auto monoReadPointer = monoBuffer.getReadPointer(0);

                std::memcpy(stereoBuffer.getWritePointer(0), monoReadPointer, buffersize * sizeof(float));
                std::memcpy(stereoBuffer.getWritePointer(1), monoReadPointer, buffersize * sizeof(float));
                
                juce::MidiBuffer dummy;
                delay.processBlock(stereoBuffer, dummy);

                writer->writeFromAudioSampleBuffer(stereoBuffer, 0, buffersize);
            }

            writer.reset();
            tempFile.overwriteTargetFileWithTemporary();
        }
    }

    return 0;
}
