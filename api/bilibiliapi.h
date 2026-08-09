#ifndef BILIBILIAPI_H
#define BILIBILIAPI_H

#include "common.h"

namespace BilibiliApi{

    QNetworkRequest weeklyPeriods();
    QNetworkRequest weeklySongs(int listId);
    QNetworkRequest videoInfoRequest(QString bvid);
}

#endif // BILIBILIAPI_H
