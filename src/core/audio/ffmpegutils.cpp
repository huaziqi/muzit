#include "ffmpegutils.h"
#include <QByteArray>

extern "C"{
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
}

namespace FFmpegUtils {

bool applyMetadata(AVFormatContext *output, const AudioMetadata metaData, QString &error)
{


    error.clear();
    if(output == nullptr){
        error = QStringLiteral("输出内容为空");
        return false;
    }

    const QByteArray artistName = metaData.artist.trimmed().toUtf8();

    int result = av_dict_set(&output->metadata,
                             "artist",
                             artistName.constData(),
                             0);

    if(result < 0){
        error = QStringLiteral("写入歌手失败");
        return false;
    }


    return true;
}


}
