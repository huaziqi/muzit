#include "audioconverttask.h"

AudioConvertTask::AudioConvertTask(
    const AudioConvertOptions &options,
    QObject *parent)
    : QObject{parent}, options{options}
{
    //定义两个函数变量，一个进度反馈，一个取消请求
    progressFunc = [this](int progressPercent){
        emit progressChanged(progressPercent);
    };
    cancelRequest = [this](){
        return cancelRequested.load();
    };
    mainProcessor = std::make_unique<AudioProcessor>(options);
}

void AudioConvertTask::start()
{
    //将两个函数变量传给processor的start函数，因为感觉放在类构造函数中不美观，options放在那里面倒是可以起到调试作用
    QString error;
    mainProcessor->start(progressFunc, cancelRequest, error);
}





void AudioConvertTask::cancel()
{
    cancelRequested.store(true);
}
