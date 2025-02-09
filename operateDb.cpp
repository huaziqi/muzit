#include "operateDb.h"

bool isTableExist(QString tableName, QSqlDatabase db){
    return db.tables().contains(tableName);
}

void singleExec(QString execContent, QSqlQuery *query){ //创建新数据表
    query->prepare(execContent);
    if(!query->exec())
        qDebug() << "Error: Fail to create table. " << query->lastError();
}

void insertTable(QString tableName, DbInfo insertContent, QSqlQuery *query){ //插入DbInfo
    query->prepare(QString("INSERT INTO %1 VALUES(?,?,?)").arg(tableName));
    query->addBindValue(0);
    query->addBindValue(insertContent.MusicListNum);
    query->addBindValue(insertContent.chosenDir);
    if(!query->exec())
        qDebug() << "Error: Fail to insert table" <<query->lastError();
}

void insertTable(QString tableName, MusicInfo insertContent, QSqlQuery *query){ //插入DbInfo
    query->prepare(QString("INSERT INTO %1 VALUES(?,?,?,?,?,?)").arg(tableName));
    query->addBindValue(insertContent.name);
    query->addBindValue(insertContent.singer);
    query->addBindValue(insertContent.album);
    query->addBindValue(insertContent.style);
    query->addBindValue(insertContent.path);
    query->addBindValue(insertContent.times);
    if(!query->exec())
        qDebug() << "Error: Fail to insert table" <<query->lastError();
}

void insertTable(QString tableName, QString insertName, int id, QSqlQuery *query){
    query->prepare(QString("INSERT INTO %1 VALUES(?)").arg(tableName));
    query->addBindValue(insertName);
    if(!query->exec())
        qDebug() << "Error: Fail to insert table" <<query->lastError();
}

void insertTable(QString tableName, QString insertName, QSqlQuery *query){
    query->prepare(QString("INSERT INTO %1 VALUES(?)").arg(tableName));
    query->addBindValue(insertName);
    if(!query->exec())
        qDebug() << "Error: Fail to insert table" <<query->lastError();
}

void queryTable(QString tableName, DbInfo *queryContent, QSqlQuery *query){
    query->exec(QString("SELECT * FROM %1").arg(tableName));
    if(!query->exec())
        qDebug() << "Error: Fail to query table. " << query->lastError();
    else{
        while(query->next()){
            queryContent->MusicListNum = query->value(1).toInt();
            queryContent->chosenDir = query->value(2).toString();
        }
    }
}

void queryTable(QString tableName, QList<MusicInfo> *queryContent, QList<QString> *paths, QSqlQuery *query){
    paths->clear();
    queryContent->clear();
    query->exec(QString("SELECT * FROM %1").arg(tableName));
    if(!query->exec())
        qDebug() << "Error: Fail to query table. " << query->lastError();
    else{
        while(query->next()){
            queryContent->append({query->value(0).toString(), query->value(1).toString(),\
            query->value(2).toString(), query->value(3).toString(), query->value(4).toString(), query->value(5).toInt()});
            paths->append(query->value(4).toString());
        }
    }
}

void queryTable(QString tableName, QList<QString>* listsName, QSqlQuery *query){
    listsName->clear();
    query->prepare(QString("SELECT * FROM %1").arg(tableName));
    if(!query->exec())
        qDebug() << "Error: Fail to query table. " << query->lastError();
    else{
        while(query->next()){
            listsName->append(query->value(0).toString());
        }
    }
}

void updateTable(QString tableName, DbInfo insertContent, QSqlQuery *query){
    query->prepare(QString("UPDATE %1 SET musicListNum = ?, chosenDir = ? WHERE id = 0").arg(tableName));
    query->addBindValue(insertContent.MusicListNum);
    query->addBindValue(insertContent.chosenDir);
    if(!query->exec())
        qDebug() << "Error: Fail to update table. " <<query->lastError();
}

void updateTable(QString tableName, MusicInfo updateContent, QSqlQuery *query){
    query->prepare(QString("UPDATE %1 SET musicName = :name, musicSinger = :singer, musicAlbum = :album, musicStyle = :style, times = :times WHERE musicPath = :path").arg(tableName));
    query->bindValue(":name", updateContent.name);
    query->bindValue(":singer", updateContent.singer);
    query->bindValue(":album", updateContent.album);
    query->bindValue(":style", updateContent.style);
    query->bindValue(":times", updateContent.times);
    query->bindValue(":path", updateContent.path);
    if(!query->exec())
        qDebug() << "Error: Fail to update table. " <<query->lastError();
}

void delTable(QString tableName, QSqlQuery *query){
    query->prepare(QString("DROP TABLE %1").arg(tableName));
    if(!query->exec())
        qDebug() << query->lastError();
}
void delFromTable(QString delList, QSqlQuery *query){
    query->prepare(QString("DELETE FROM musicListInfo WHERE musicListName=:name"));
    query->bindValue(":name", delList);
    if(!query->exec())
        qDebug() << query->lastError();
}

void delFromTable(QString tableName, QString delUrl, QSqlQuery *query){
    query->prepare(QString("DELETE FROM %1 WHERE musicPath=:name").arg(tableName));
    query->bindValue(":name", delUrl);
    if(!query->exec())
        qDebug() << query->lastError();
}


