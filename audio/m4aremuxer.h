#ifndef M4AREMUXER_H
#define M4AREMUXER_H

#include "audioconverter.h"

class M4aRemuxer : public AudioConverter
{
public:
    M4aRemuxer();
    bool convert(
        const AudioConvertOptions &options,
        const ProgressCallback &progress,
        const CancellationRequest &cancelRequest,
        QString &error
    ) override;
};

#endif // M4AREMUXER_H
