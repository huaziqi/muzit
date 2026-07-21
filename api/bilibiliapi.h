#ifndef BILIBILIAPI_H
#define BILIBILIAPI_H

#include "common.h"

namespace BilibiliApi{

    QNetworkRequest weeklyPeriods();
    QNetworkRequest weeklySongs(int listId);
}

#endif // BILIBILIAPI_H
