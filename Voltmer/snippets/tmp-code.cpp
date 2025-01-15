audiumEngine.getPlayListScheduler()->prepareToPlay(config.blockSize, config.sampleRate);
    
TemporaryFile tempFile (config.fileName);
std::unique_ptr<OutputStream> outStream (tempFile.getFile().createOutputStream());

if (outStream != nullptr) {
    const StringPairArray metadata;
    WavAudioFormat wav;
    std::unique_ptr<AudioFormatWriter> writer (wav.createWriterFor (outStream.release(), config.sampleRate,
                                                                    (unsigned int)config.numChannels,
                                                                    config.bitDepth,
                                                                    metadata, 0));
    if (writer != nullptr) {
        audiumEngine.getPlayListScheduler()->bounceToFile(writer.get(), config, [this](void) {
            setProgress(this->config.progress);
        });

        writer.reset();
        tempFile.overwriteTargetFileWithTemporary();
    }
}

