#ifndef FFMPEGUTILS_H
#define FFMPEGUTILS_H

#include "core/metadata/metadatatypes.h"

struct AVFormatContext;

namespace FFmpegUtils {
bool applyMetadata(AVFormatContext* output,
                   const AudioMetadata metaData,
                   QString& error);
}


#endif // FFMPEGUTILS_H
