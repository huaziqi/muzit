#ifndef COMMON_H
#define COMMON_H


#include <QCoreApplication>
//文件处理
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
//事件处理
#include <QMouseEvent>
#include <QResizeEvent>

//数学
#include <QtMath>

//页面布局
#include <QMenu>
#include <QWidget>
#include <QFrame>
#include <QScrollArea>
#include <QPoint>
#include <QVBoxLayout>
#include <QHBoxLayout>

//控件
#include <QStackedWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>


//UI
#include <QPainter>
#include <QColor>
#include <QIcon>
#include <QPainterPath>
#include <QFontDatabase>

#include <QApplication>
#include <QPropertyAnimation>

//结构体
#include <QVector>

//网络
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace common {
    struct SongInfo{
        int rank;
        int playedNum, duration; //播放量和持续时间
        QString BVId;
        QString title, author;
        QString coverUrl;

        SongInfo(int _rank, int _playedNum, int _duration, QString _BVId, QString _title, QString _author, QString _coverUrl)\
            : rank(_rank), playedNum(_playedNum), duration(_duration), BVId(_BVId), title(_title), author(_author), coverUrl(_coverUrl){}
    };

    inline bool loadFont(const QString& fontPath){ //加载字体
        int loadFontId = QFontDatabase::addApplicationFont(fontPath);
        if(loadFontId == -1){
            return false;
        }
        QStringList fontFamliy = QFontDatabase::applicationFontFamilies(loadFontId);
        if(fontFamliy.empty()){
            qDebug() << "[common.h] 字体" << fontPath << "为空";
            return false;
        }
        qDebug() << fontFamliy.at(0);
        return true;

    }
    inline QFont vonwaoFont = QFont("VonwaonBitmap 16px", 16);
}

#endif // COMMON_H
