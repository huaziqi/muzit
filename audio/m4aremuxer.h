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
        const ProcessCallback &process,
        const CancellationCallback &cancel,
        const QString &error
    );
};

#endif // M4AREMUXER_H
