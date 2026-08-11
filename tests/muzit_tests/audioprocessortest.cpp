#include "audioprocessortest.h"

#include "audioprocessor.h"

#include <QDebug>

bool AudioProcessorTest::run(const QString &inputPath)
{
    AudioConvertOptions options;
    options.inputPath = inputPath;
    options.outputPath = QStringLiteral("D:/Music/output.m4a");

    AudioProcessor processor;
    QString error;

    const bool success = processor.process(
        options,
        [](int percent) {
            qDebug() << "Progress:" << percent << "%";
        },
        []() {
            return false;
        },
        error);

    if (success) {
        qDebug() << "Audio input opened successfully.";
        return true;
    }

    qCritical() << "Audio input failed:" << error;
    return false;
}
