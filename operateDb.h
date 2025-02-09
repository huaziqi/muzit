#ifndef OPERATEDB_H
#define OPERATEDB_H


#include "common.h"
#include "structs.h"

bool isTableExist(QString tableName, QSqlDatabase db);
void singleExec(QString tableInfo, QSqlQuery *query);
void insertTable(QString tableName, DbInfo insertContent, QSqlQuery *query);
void insertTable(QString tableName, MusicInfo insertContent, QSqlQuery *query);
void insertTable(QString tableName, QString insertName, int id, QSqlQuery *query);
void insertTable(QString tableName, QString insertName, QSqlQuery *query);
void queryTable(QString tableName, DbInfo *queryContent, QSqlQuery *query);
void queryTable(QString tableName, QList<MusicInfo> *queryContent, QList<QString> *paths, QSqlQuery *query);
void queryTable(QString tableName, QList<QString>* listsName, QSqlQuery *query);
void updateTable(QString tableName, DbInfo insertContent, QSqlQuery *query);
void updateTable(QString tableName, MusicInfo updateContent, QSqlQuery *query);
void delTable(QString tableName, QSqlQuery *query);
void delFromTable(QString delList, QSqlQuery *query);
void delFromTable(QString tableName, QString delUrl, QSqlQuery *query);

#endif // OPERATEDB_H
