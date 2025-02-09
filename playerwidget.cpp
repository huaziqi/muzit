#include "playerwidget.h"
#include "ui_playerwidget.h"
#include <QDebug>

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerWidget)
{

    ui->setupUi(this);
    hideParts();
    ui->listWidget->setAlternatingRowColors(true);//设置列表的每行颜色相间
    connect(ui->selectDirBtn, &QPushButton::clicked, this, &PlayerWidget::selectDirBtn_clicked);
    if(!inited)
        init(), inited = true;
}

PlayerWidget::~PlayerWidget(){
    delete ui;
}

void PlayerWidget::hideParts(){
    ui->delListBtn->hide();
    ui->choseListBox->hide();
    ui->addToListBtn->hide();
    ui->delChosenBtn->hide();
    ui->playChosenBtn->hide();
    ui->updInfoBtn->hide();
    ui->searchLine->hide();
    ui->searchBtn->hide();
    ui->selectDirBtn->hide();
    ui->delFromListBtn->hide();
}

void PlayerWidget::showParts(){
    ui->choseListBox->show();
    ui->addToListBtn->show();
    ui->delChosenBtn->show();
    ui->playChosenBtn->show();
    ui->updInfoBtn->show();
    ui->searchLine->show();
    ui->searchBtn->show();
    ui->selectDirBtn->show();
}

void PlayerWidget::init(){
    ui->listWidget->setMovement(QListView::Static);
    gMenuList();
    initDB();
    if(!dirPath.exists())
        dirPath.mkpath(".");
}

void PlayerWidget::initDB(){
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("MusicInfo.db");
    DB.open();
    sqlQuery = new QSqlQuery;
    if(!isTableExist("dbInfo", DB)){
        singleExec(QString("CREATE TABLE dbInfo (\
                            id INT PRIMARY KEY NOT NULL, \
                            musicListNum INT NOT NULL, \
                            chosenDir TEXT NOT NULL)"), sqlQuery);

        insertTable("dbInfo", (DbInfo){0, "D:\\music"}, sqlQuery);
        musicListNum = 0;
        dirPath = QDir("D:\\music");
        singleExec(QString("CREATE TABLE musicListInfo (\
                            musicListName TEXT PRIMARY KEY NOT NULL)"), sqlQuery);
        singleExec(QString("CREATE TABLE musicInfo (\
                            musicName TEXT NOT NULL, \
                            musicSinger TEXT  NOT NULL, \
                            musicAlbum TEXT NOT NULL,\
                            musicStyle TEXT NOT NULL,\
                            musicPath TEXT PRIMARY KEY NOT NULL,\
                            times INT NOT NULL)"), sqlQuery);
    }
    else{
        queryTable("dbInfo", &dbContent, sqlQuery);
        musicListNum = dbContent.MusicListNum;
        dirPath = QDir(dbContent.chosenDir);
    }
}

void PlayerWidget::gMenuList(){
    //qDebug() << 123;
    ui->listWidget->clear();
    ui->listWidget->setViewMode(QListView::IconMode);
    QFont font = ui->listWidget->font();
    font.setPointSize(12);
    ui->listWidget->setFont(font);
    ui->listWidget->setIconSize(QSize(50, 50));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/assets/all_music.png"), "所有音乐"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/assets/music_lists.png"), "歌单"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/assets/singer_lists.png"), "歌手"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/assets/album_lists.png"), "专辑"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":/assets/style_lists.png"), "风格"));
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::menuList_clicked);
}

bool cmp0(MusicInfo a, MusicInfo b){
    return a.times < b.times;
}

void PlayerWidget::gAllMusic(){
    scanfMusic(&musicPaths);
    syncMusic();
    std::sort(musicInfo.begin(), musicInfo.end(), cmp0);
    showMusic();
}

void PlayerWidget::gClassedMusic(){ //三个分类页面
    ui->listWidget->clear();
    hideParts();
    scanfMusic(&musicPaths);
    syncMusic();
    classedInfo.clear();
    QString keyString;
    for(int i = 0; i < musicInfo.count(); i ++){ //获取所有分类
        if(classStd == "singer")
            keyString = musicInfo.value(i).singer;
        else if(classStd == "album")
            keyString = musicInfo.value(i).album;
        else if(classStd == "style")
            keyString = musicInfo.value(i).style;
        classedInfo[keyString].append(musicInfo.value(i));
    }

    ui->listWidget->addItem(new QListWidgetItem("返回.."));
    for (const QString &key : classedInfo.keys()) {
        ui->listWidget->addItem(new QListWidgetItem(key)); //将分类加入列表
    }
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::classedMenu_clicked);
}

void PlayerWidget::gListPage(){ //歌单页面
    ui->listWidget->addItem(new QListWidgetItem("返回.."));
    queryTable("musicListInfo", &listsName, sqlQuery);
    for(int i = 0; i < listsName.count(); i ++)
        ui->listWidget->addItem(new QListWidgetItem(listsName.value(i)));
    ui->listWidget->addItem(new QListWidgetItem("+新建+"));
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::listPage_clicked);
}

void PlayerWidget::scanfMusic(QList<QString> *paths){
    *paths = dirPath.entryList(QStringList() << "*.mp3");
}

void PlayerWidget::syncMusic(){
    queryTable("musicInfo", &musicInfo, &inDbPaths, sqlQuery); //获得数据哭中的音乐信息
    for(int i = 0; i < musicPaths.count(); i ++){ //若有库中不存在的就插入
        if(!inDbPaths.contains(musicPaths.value(i))){
            QString temName = musicPaths.value(i);
            temName.chop(4);
            insertTable("musicInfo", (MusicInfo){temName, "未知", "未知", "未知", musicPaths.value(i), 0}, sqlQuery);
        }
    }
    queryTable("musicInfo", &musicInfo, &inDbPaths, sqlQuery);//重新获取
    //QMap <QString, MusicInfo> itemInfo;
    itemInfo.clear();
    for(int i = 0; i < musicInfo.count(); i ++){
        if(!musicPaths.contains(musicInfo.value(i).path)){//本地不存在
            musicInfo.removeAt(i);
            i --;
        }
        else{
            itemInfo.insert(musicInfo.value(i).name, musicInfo.value(i));//插入到Map中，方便后面获取数据
        }
    }
}

void PlayerWidget::showMusic(){ //展示所有音乐
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    showParts();
    ui->listWidget->clear();
    ui->listWidget->addItem(new QListWidgetItem("返回.."));
    for(int i = 0; i < musicInfo.count(); i ++){
        ui->listWidget->addItem(new QListWidgetItem(musicInfo.value(i).name));
    }
    queryTable("musicListInfo", &listsName, sqlQuery);
    ui->choseListBox->clear();
    if(listsName.empty())
        ui->choseListBox->addItem("无歌单");
    else{
        for(int i = 0; i < listsName.count(); i ++)
            ui->choseListBox->addItem(listsName.value(i));
    }
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::musicList_clicked);
}

void PlayerWidget::showMusic(QString nowList){ //生成歌单中的音乐，因为功能按钮有区别
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->searchLine->show();
    ui->searchBtn->show();
    ui->selectDirBtn->show();
    ui->playChosenBtn->show();
    ui->delFromListBtn->show();
    ui->listWidget->clear();
    ui->delListBtn->show();
    ui->listWidget->addItem(new QListWidgetItem("返回.."));
    for(int i = 0; i < musicInfo.count(); i ++){
        ui->listWidget->addItem(new QListWidgetItem(musicInfo.value(i).name));
    }
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::musicList_clicked);
}

void PlayerWidget::selectDirBtn_clicked(){ //选择文件夹
    dirPath = QFileDialog::getExistingDirectory(this, "选择音乐文件夹", "D:\\music");
    if(dirPath.path() != ".")
        updateTable("dbInfo", {musicListNum, dirPath.path()}, sqlQuery);
    else
        dirPath = QDir("D:\\music");
    scanfMusic(&musicPaths);
    syncMusic();
    showMusic();
}

void PlayerWidget::menuList_clicked(){ //点击菜单页面
    int row = ui->listWidget->currentRow();
    disconnect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::menuList_clicked);
    ui->listWidget->clear();
    ui->listWidget->setViewMode(QListView::ListMode);
    if(row == 0){
        nowPage = 1;
        gAllMusic();
    }
    else if(row >= 2 && row <= 4){
        if(row == 2)
            classStd = "singer";
        else if(row == 3)
            classStd = "album";
        else
            classStd = "style";
        nowPage = 2;
        gClassedMusic();
    }
    else{
        nowPage = 3;
        gListPage();
    }
}

void PlayerWidget::musicList_clicked(){ //点击音乐列表
    int row = ui->listWidget->currentRow();
    if(row == 0){
        disconnect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::musicList_clicked);
        ui->listWidget->clear();
        gMenuList();
        ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        hideParts();
    }
    else{
        on_playChosenBtn_clicked();
    }
}

void PlayerWidget::classedMenu_clicked(){ //点击分类页面
    int row = ui->listWidget->currentRow();
    disconnect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::classedMenu_clicked);
    shuffleString = ui->listWidget->currentItem()->text();
    ui->listWidget->clear();
    if(row == 0)
        gMenuList();
    else{
        musicInfo = classedInfo[shuffleString]; //筛选
        showMusic();
        ui->updInfoBtn->hide();
    }
}

void PlayerWidget::listPage_clicked(){ //歌单页面点击
    int row = ui->listWidget->currentRow();
    nowList = ui->listWidget->currentItem()->text();
    int num = ui->listWidget->count();
    if(row == num - 1){
        QLineEdit::EchoMode echoMode=QLineEdit::Normal;
        bool ok = false;
        QString name = QInputDialog::getText(this, "输入歌单名称","输入歌单名称", echoMode, "" , &ok);
        if(name == "")
            return;
        if(isTableExist(name, DB))
            QMessageBox::information(this, "warning", "歌单已存在");
        else if(name[0] >= '0' && name[0] <= '9')
            QMessageBox::information(this, "warning", "歌单名称违规");
        else{
            insertTable("musicListInfo", name, musicListNum, sqlQuery);
            musicListNum ++;
            singleExec(QString("CREATE TABLE %1 (\
                                musicPath TEXT PRIMARY KEY NOT NULL)").arg(name), sqlQuery);
            ui->listWidget->clear();
            gMenuList();
        }
    }
    else{
        disconnect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::listPage_clicked);
        ui->listWidget->clear();
        if(row == 0)
            gMenuList();
        else{
            queryTable(nowList, &listPaths, sqlQuery);
            scanfMusic(&musicPaths);
            syncMusic();
            for(int i = 0; i < musicInfo.count(); i ++){
                if(!listPaths.contains(musicInfo.value(i).path))
                    musicInfo.removeAt(i), i --;
            }
            showMusic(nowList);
        }
    }
}

void PlayerWidget::on_playChosenBtn_clicked(){ //播放所选音乐
    QList<MusicInfo> newMusic;
    for(int i = 0; i < ui->listWidget->selectedItems().count(); i ++){
        if(ui->listWidget->selectedItems().value(i)->text() == "返回..")
            continue;
        newMusic.append(itemInfo[ui->listWidget->selectedItems().value(i)->text()]);
        itemInfo[ui->listWidget->selectedItems().value(i)->text()].times ++;
        updateTable("musicInfo", itemInfo[ui->listWidget->selectedItems().value(i)->text()], sqlQuery);
    }
    emit sendUrl(newMusic, dirPath);
}

void PlayerWidget::on_delChosenBtn_clicked(){
    for(int i = 0; i < ui->listWidget->selectedItems().count(); i ++){
        if(ui->listWidget->selectedItems().value(i)->text() == "返回..")
            continue;
        QFile delfile = QFile(dirPath.path() + "/" +itemInfo[ui->listWidget->selectedItems().value(i)->text()].path);
        if(delfile.exists())
            delfile.remove();
    }
    scanfMusic(&musicPaths);
    syncMusic();
    if(nowPage == 1)
        showMusic();
    if(nowPage == 2)
        gClassedMusic();

}

bool cmp1(MusicInfo a, MusicInfo b){
    return a.weight > b.weight;
}

void PlayerWidget::on_searchBtn_clicked(){ //搜索音乐
    QString searchText = ui->searchLine->text();
    scanfMusic(&musicPaths);
    syncMusic();
    if(searchText == ""){
        showMusic();
    }
    else{
        if(searchText[0] == '!'){ //高级搜索功能，不显示含有相关信息的音乐
            searchText.removeAt(0);
            for(int i = 0; i < musicInfo.count(); i ++){
                if(musicInfo.value(i).name.contains(searchText) || musicInfo.value(i).singer.contains(searchText) || musicInfo.value(i).album.contains(searchText) || \
                    musicInfo.value(i).style.contains(searchText))
                    musicInfo.removeAt(i), i --;
            }
        }
        else{
            for(int i = 0; i < musicInfo.count(); i ++){
                musicInfo[i].weight = 0;
            }
            for(int i = 0; i < musicInfo.count(); i ++){
                bool searchable = false;
                if(musicInfo.value(i).name.contains(searchText))
                    musicInfo[i].weight += 10, searchable = true;
                if(musicInfo.value(i).singer.contains(searchText))
                    musicInfo[i].weight += 5, searchable = true;
                if(musicInfo.value(i).album.contains(searchText))
                    musicInfo[i].weight += 5, searchable = true;
                if(musicInfo.value(i).style.contains(searchText))
                    musicInfo[i].weight += 5, searchable = true;
                if(!searchable)
                    musicInfo.removeAt(i), i --;
            }
            std::sort(musicInfo.begin(), musicInfo.end(), cmp1);
        }
        showMusic();
    }
}

void PlayerWidget::on_addToListBtn_clicked(){ //添加到歌单
    if(listsName.empty())
        QMessageBox::information(this, "warn", "无可用歌单");
    else{
        for(int i = 0; i < ui->listWidget->selectedItems().count(); i ++){
            QString addedUrl = itemInfo[ui->listWidget->selectedItems().value(i)->text()].path;
            QString listname = ui->choseListBox->currentText();
            queryTable(listname, &listsName, sqlQuery);
            if(!listsName.contains(addedUrl))
                insertTable(listname, addedUrl, sqlQuery);
        }
    }
}

void PlayerWidget::on_delListBtn_clicked(){//删除该歌单
    delTable(nowList, sqlQuery);
    delFromTable(nowList, sqlQuery);
    disconnect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PlayerWidget::musicList_clicked);
    ui->listWidget->clear();
    gMenuList();
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    hideParts();
}

void PlayerWidget::on_updInfoBtn_clicked(){ //更新信息
    auto items = ui->listWidget->selectedItems();
    if(!items.empty()){
        QDialog dialog(this);
        QFormLayout form(&dialog);
        QList<QLineEdit *> fields;
        if(items[0]->text() == "返回..")
            items.removeAt(0);
        if(items.count() == 0){
            return;
        }
        form.addRow(new QLabel("修改信息"));
        bool fl = (items.count() == 1);
        QLineEdit *lineEdit;
        if(fl){
            lineEdit = new QLineEdit(&dialog);
            QString label = QString("歌名");
            form.addRow(label, lineEdit);
            fields << lineEdit;
        }
        QLineEdit *lineEdit0 = new QLineEdit(&dialog);QString label0 = QString("歌手");form.addRow(label0, lineEdit0);fields << lineEdit0;
        QLineEdit *lineEdit1 = new QLineEdit(&dialog);QString label1 = QString("专辑");form.addRow(label1, lineEdit1);fields << lineEdit1;
        QLineEdit *lineEdit2 = new QLineEdit(&dialog);QString label2 = QString("风格");form.addRow(label2, lineEdit2);fields << lineEdit2;
        if(fl){
            MusicInfo updateContent = itemInfo[items.value(0)->text()];
            lineEdit->setPlaceholderText(updateContent.name);lineEdit0->setPlaceholderText(updateContent.singer);lineEdit1->setPlaceholderText(updateContent.album);lineEdit2->setPlaceholderText(updateContent.style);
            //qDebug() <<  updateContent.style;
        }
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        if (dialog.exec() == QDialog::Accepted) {
            for(int i = 0; i < items.count(); i ++){
                MusicInfo updateContent = itemInfo[items.value(i)->text()];
                int num = 0;
                foreach(QLineEdit * lineEdit, fields) {
                    num ++;
                    if(lineEdit->text() == "")
                        continue;
                    else{
                        //bool fl = (items.count() == 1);
                        if(num - fl == 0){
                            if(itemInfo.keys().contains(lineEdit->text())){
                                QMessageBox::information(this, "warning", "禁止重名");
                                return;
                            }
                            updateContent.name = lineEdit->text();
                        }
                        else if(num - fl == 1)
                            updateContent.singer = lineEdit->text();
                        else if(num - fl == 2)
                            updateContent.album = lineEdit->text();
                        else if(num - fl == 3)
                            updateContent.style = lineEdit->text();
                    }
                }
                updateTable("musicInfo", updateContent, sqlQuery);
            }
        }
        gAllMusic();
    }
}

void PlayerWidget::on_delFromListBtn_clicked(){ //从歌单中删除
    auto items = ui->listWidget->selectedItems();
    for(int i = 0; i < items.count(); i ++){
        if(items.value(i)->text() == "返回..")
            continue;
        delFromTable(nowList, itemInfo[items.value(i)->text()].path, sqlQuery);
    }
    queryTable(nowList, &listPaths, sqlQuery);
    scanfMusic(&musicPaths);
    syncMusic();
    for(int i = 0; i < musicInfo.count(); i ++){
        if(!listPaths.contains(musicInfo.value(i).path))
            musicInfo.removeAt(i), i --;
    }
    showMusic(nowList);
}

