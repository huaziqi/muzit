#include "config.h"

Config& Config::instance(){
    static Config inst;
    return inst;
}

Config::Config()
    : m_settings(QSettings::IniFormat, QSettings::UserScope, "transpire", "muzit") {
    qDebug() << "配置文件路径：" << m_settings.fileName();
}

Config::~Config(){
    QWriteLocker locker(&m_lock);
    m_settings.sync();
}

QVariant Config::getValuewithGroup(const QString& settingsGroup, const QString& key, const QVariant& defaultValues) const{
    QReadLocker locker(&m_lock);
    m_settings.beginGroup(settingsGroup);
    QVariant result = m_settings.value(key, defaultValues);
    m_settings.endGroup();

    return result;
}

void Config::setValue(const QString& key, const QVariant& value){
    {
        QWriteLocker locker(&m_lock);
        m_settings.setValue(key, value);
    }
    emit settingsChanged(key, value);
}

void Config::setValuewithGroup(const QString& settingsGroup, const QString& key, const QVariant& value){
    {
        QWriteLocker locker(&m_lock);
        m_settings.beginGroup(settingsGroup);
        m_settings.setValue(key, value);
        m_settings.endGroup();
    }
    emit settingsChanged(settingsGroup, key, value);
}
