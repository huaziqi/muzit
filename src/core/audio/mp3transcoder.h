#ifndef MP3TRANSCODER_H
#define MP3TRANSCODER_H

#include "audioconverter.h"

class Mp3Transcoder : public AudioConverter
{
public:
    Mp3Transcoder();

    bool convert(
        const AudioConvertOptions &options,
        const ProgressCallback &progress,
        const CancellationRequest &cancelRequest,
        QString &error
    ) override;

private:
    struct TranscodeContext;

    void initializeInput(
        TranscodeContext &context,
        const AudioConvertOptions &options);

    void initializeOutput(
        TranscodeContext &context,
        const AudioConvertOptions &options);

    void initializeResampler(TranscodeContext &context);
    void initializeBuffers(TranscodeContext &context);

    bool transcodePackets(
        TranscodeContext &context,
        const CancellationRequest &cancelRequest);

    void receiveDecodedFrames(TranscodeContext &context);
    int resampleToFifo(TranscodeContext &context, bool draining);

    void encodeAvailableSamples(
        TranscodeContext &context,
        bool includeRemainingSamples);

    void encodeOneFrame(TranscodeContext &context, int sampleCount);
    void writeEncodedPackets(TranscodeContext &context);
    void flushDecoder(TranscodeContext &context);
    void flushResampler(TranscodeContext &context);
    void flushEncoder(TranscodeContext &context);
};

#endif // MP3TRANSCODER_H
