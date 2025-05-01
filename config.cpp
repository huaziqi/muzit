#include "config.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>

void Config::initConfig(const QString& filePath){
    QFile configFile = QFile(filePath);
    QJsonObject configJson;
    configJson["colorPlan"] = "normal";
    configJson["read"] = "true";
    QJsonArray testArray;
    testArray.append("1");
    testArray.append("2");
    testArray.append("3");
    configJson["testArray"] = testArray;
    QJsonDocument jsonDoc(configJson);
    if(configFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        configFile.write(jsonDoc.toJson(QJsonDocument::Indented));
        configFile.close();
        qDebug() << "文件创建在" << filePath;
    }
    else{
        qDebug() << "文件打开失败";
    }
}

void Config::readConfig()
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString configPath = configDir + "/config.json";
    QFile configFile = QFile(configPath);
    if(configFile.exists())
        qDebug() << "";
    else{
        if(!QDir(configDir).exists()){
            if(QDir().mkdir(configDir)) initConfig(configPath);
            else QMessageBox::warning(nullptr, "警告", "文件夹创建失败");
        }
        else
            initConfig(configPath); //是否存在其他应用配置文件夹同名？maybe
    }
}

Config::Config() {}
