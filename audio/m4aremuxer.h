#ifndef M4AREMUXER_H
#define M4AREMUXER_H

#include <QObject>
#include "audioconverter.h"

class M4aRemuxer : public AudioConverter
{
    Q_OBJECT
public:
    M4aRemuxer();
    bool convert(
        const AudioConvertOptions &option,
        const ProgressCallback &progress,
        const CancellationRequest &cancelRequest,
        QString &error
    ) override;
};

#endif // M4AREMUXER_H
