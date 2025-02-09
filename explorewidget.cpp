#include "explorewidget.h"
#include "ui_explorewidget.h"
#include "operateDb.h"



ExploreWidget::ExploreWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExploreWidget)
{
    ui->setupUi(this);
    connect(ui->searchBtn, &QPushButton::clicked, this, &ExploreWidget::startSearch);
    ui->titleWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

ExploreWidget::~ExploreWidget()
{
    delete ui;
}

void ExploreWidget::startSearch(){

    urls.clear();
    if(ui->bvRadio->isChecked()){
        QString bvId = ui->searchContent->text();
        searchTitleByBv(bvId);
    }
    else if(ui->titleRadio->isChecked()){
        ui->titleWidget->addItem(new QListWidgetItem("搜索中..."));
        searchByTitle(ui->searchContent->text());
    }
    ui->titleWidget->clear();
    for (const QString &key : urls.keys()) {
        if(key == "")
            continue;
        ui->titleWidget->addItem(new QListWidgetItem(key));
    }
}
void ExploreWidget::downLoadByBv(QString title, QString bvId, QString cId){ //通过bvId和cId下载
    //获取音频地址
    //qDebug() << bvId << cId;
    QNetworkAccessManager networkManager;
    QUrl sourceUrl("https://api.bilibili.com/x/player/playurl?fnval=16&bvid=" + bvId +"&cid=" + cId);
    QNetworkReply *apiReply = networkManager.get(QNetworkRequest(sourceUrl));
    QEventLoop apiLoop;
    QObject::connect(apiReply, SIGNAL(finished()), &apiLoop, SLOT(quit()));
    apiLoop.exec();

    QString sourceContent = apiReply->readAll();

    QJsonDocument jsonData = QJsonDocument::fromJson(sourceContent.toUtf8());
    QString audioText = jsonData["data"]["dash"]["audio"][0]["baseUrl"].toString();
    QUrl audioUrl(audioText);

    //下载音频
    QNetworkRequest audioRequest(audioUrl);
    audioRequest.setRawHeader("Referer", "https://www.bilibili.com/");
    audioRequest.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36");//Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36
    QNetworkReply *audioReply = networkManager.get(audioRequest);

    QEventLoop audioLoop;
    QObject::connect(audioReply, SIGNAL(finished()), &audioLoop, SLOT(quit()));
    audioLoop.exec();
    QByteArray  mp3Info = audioReply->readAll();
    QFile audioFile(QString(downPath.path() + "/%1.mp3").arg(title));
    audioFile.open(QIODevice::WriteOnly);
    audioFile.write(mp3Info);
    audioFile.close();
}

void ExploreWidget::searchTitleByBv(QString bvId){
    QEventLoop bvLoop;
    QUrl url("https://www.bilibili.com/video/" + bvId + "/");
    QNetworkAccessManager networkManager;
    QNetworkReply *bvReply = networkManager.get(QNetworkRequest(url));
    QObject::connect(bvReply, SIGNAL(finished()), &bvLoop, SLOT(quit()));
    bvLoop.exec();
    QString html = bvReply->readAll();
    QRegularExpression cidRegex("\"cid\":([0-9]*)");
    QRegularExpression titleRegex("\"title\":\"(.+?)\"");
    QRegularExpressionMatch match0 = cidRegex.match(html);
    QRegularExpressionMatch match1 = titleRegex.match(html);
    QString cId = match0.captured(0).remove(0, 6);
    QString musicTitle = match1.captured(0);
    musicTitle.remove(0, 9), musicTitle.chop(1);
    musicTitle.replace(QRegularExpression("[|\\/:\\*\\\\?\\<>]"), "");
    //qDebug() << musicTitle;
    urls.insert(musicTitle, {bvId, cId});
}

void ExploreWidget::searchByTitle(QString sTitle){
    QEventLoop titleLoop;
    QUrl url("https://search.bilibili.com/all?keyword=" + sTitle + "/");
    //qDebug() << 1;
    QNetworkAccessManager networkManager;
    QNetworkReply *titleReply = networkManager.get(QNetworkRequest(url));
    QObject::connect(titleReply, SIGNAL(finished()), &titleLoop, SLOT(quit()));
    titleLoop.exec();
    QString html = titleReply->readAll();
    QRegularExpression titleRegex("bvid:\"(.*?)\"");
    //qDebug() << 2;
    QRegularExpressionMatchIterator it = titleRegex.globalMatch(html);
    int num = 0;
    while(it.hasNext()){
        if(num == ui->numSpin->value())
            break;
        num ++;
        QRegularExpressionMatch match = it.next();
        QString temStr = match.captured(0);
        temStr.remove(0, 6), temStr.chop(1);
        searchTitleByBv(temStr);
    }
}

void ExploreWidget::on_selectDirBtn_clicked(){
    downPath = QFileDialog::getExistingDirectory(this, "选择音乐文件夹", "D:\\music");
    if(downPath.path() == ".")
        downPath = QDir("D:\\music");
}


void ExploreWidget::on_downloadBtn_clicked(){
    auto items = ui->titleWidget->selectedItems();
    for (const QString &key : urls.keys()) {
        for(int i = 0; i < items.count(); i ++)
            if(key == items.value(i)->text()){
                downLoadByBv(key, urls.value(key).value(0), urls.value(key).value(1));
                break;
            }
    }
}

