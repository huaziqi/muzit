#include "audioprocessortest.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    const QStringList arguments = app.arguments();

    if (arguments.size() != 2) {
        qCritical() << "Usage: muzit_tests <input-audio-path>";
        return 2;
    }

    AudioProcessorTest test;
    return test.run(arguments.at(1)) ? 0 : 1;
}
