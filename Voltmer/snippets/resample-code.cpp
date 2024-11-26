
/// Resample the voice
void PlaybackVoice::resample(float **data, const u_int32_t channels,
                             const u_int32_t inNumberFrames)
{
    if (processing.load() &&
        playbackFile != nullptr)
    {
        
        auto numberOfSamples = playbackFile->getNumberOfSamples();

        int samplesNeeded = static_cast<int>(static_cast<double>(inNumberFrames) * playbackFactor);

        samplesNeeded += 2; // add 2 samples here due to interpolation

        if (samplePosition + samplesNeeded <= numberOfSamples)
        {
            double accurate = 0.0;
            
            for (auto c = 0; c < channels; ++c)
            {
                const float* sampleData = playbackFile->getBuffer(c);
                accurate = accurateSamplePosition;
                samplePosition = std::trunc(accurate);
                
                for (auto k = 0; k < inNumberFrames; ++k)
                {
                    // in case we are at position 0 we must not access sample index -1 (needed by interpolation)
                    const int histOff = samplePosition == 0 ? 0 : 1;

                    // access data values for 4-point interpolation [-1, 0, 1, 2]
                    const float xm1 = sampleData[samplePosition - histOff];
                    const float x0  = sampleData[samplePosition];
                    const float x1  = sampleData[samplePosition + 1];
                    const float x2  = sampleData[samplePosition + 2];

                    // the factional part
                    const float frac = static_cast<float>(accurate) - static_cast<float>(samplePosition);
                    // increment the data pointer and assign the interpolated value
                    data[c][k] += cubicInterp(frac, xm1, x0, x1, x2);

                    // increment the play position by one tick (playbackfactor)
                    // this controls the play back speed !!!!!!
                    accurate += playbackFactor;

                    // tuncate so we can use it as index to access the sample buffer
                    samplePosition = std::trunc(accurate);

                }
                
            }
            
            accurateSamplePosition = accurate;
            samplePosition = std::trunc(accurate);
        }
        else
        {
            // we MUST stop playback in case there are not enough samples to fill the buffer.
            stop(true);
        }
    }
}



/// Interpolation function used for the resampling
inline float cubicInterp(float x, float y0, float y1, float y2, float y3)
{
    // 4-point, 3rd-order Hermite (x-form)
    const float c0 = y1;
    const float c1 = 0.5f * (y2 - y0);
    const float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
    const float c2 = y0 - y1 + c1 - c3;

    return ((c3 * x + c2) * x + c1) * x + c0;
}



