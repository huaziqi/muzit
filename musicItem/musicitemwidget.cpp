#include "musicitemwidget.h"

MusicItemWidget::MusicItemWidget(MusicItem* _musicItem, QWidget *parent)
    : QWidget{parent}
{
    musicItem = _musicItem;
    mainLayout = new QHBoxLayout();
    manager = new QNetworkAccessManager;


}
