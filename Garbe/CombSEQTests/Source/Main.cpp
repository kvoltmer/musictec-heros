/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayProcessor.h"
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
            (unsigned int)1,
            24,
            metadata, 0));
        if (writer != nullptr) {

            AudioBuffer<float> temp(1, 512);
            temp.clear();

            SinePulse sin(440.f, 500);
            sin.prepareToPlay(samplerate, buffersize);

            DelayProcessor del;
            del.prepareToPlay(samplerate, buffersize);

            double seconds = 3;
            int sampleCount = seconds * samplerate;
            int iterations = sampleCount / buffersize;

            auto ptr = temp.getWritePointer(0);

            for (auto i = 0; i < iterations; i++) {
                temp.clear();

                for (auto s = 0; s < buffersize; s++){
                    auto in = sin.gen();
                    auto out = del.process(in);
                    ptr[s] = out;
                }

                writer->writeFromAudioSampleBuffer(temp, 0, buffersize);
            }

            writer.reset();
            tempFile.overwriteTargetFileWithTemporary();
        }
    }

    return 0;
}
