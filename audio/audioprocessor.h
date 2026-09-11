#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include "audioconverter.h"
#include <QObject>

class AudioProcessor : public QObject
{
    Q_OBJECT
public:
    explicit AudioProcessor(const AudioConvertOptions& options, QObject* parent = nullptr);
    std::unique_ptr<AudioConverter> createConverter(const AudioOutputFormat& format);
    void start(const AudioConverter::ProgressCallback& progressFunc,
               const AudioConverter::CancellationRequest& cancelRequest,
               QString& error);

private:
    void startConvert(const AudioConvertOptions& options,
                      const AudioConverter::ProgressCallback& progressFunc,
                      const AudioConverter::CancellationRequest& cancelRequest,
                      QString& error);
    std::unique_ptr<AudioConverter> mainConverter;
    AudioConvertOptions audioOptions;
};

#endif // AUDIOPROCESSOR_H
