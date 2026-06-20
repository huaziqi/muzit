#ifndef CONFIG_H
#define CONFIG_H
#include "common.h"
#include <QSettings>
#include <QReadWriteLock>
#include <QVariant>

class Config : public QObject
{
    Q_OBJECT
public:
    static Config& instance();
    Config(const Config&) = delete;
    Config& operator=(Config&) = delete;

private:
    Config();
    ~Config();
    mutable QSettings m_settings;
    mutable QReadWriteLock m_lock;

public:
    QVariant getValuewithGroup(const QString& settingsGroup, const QString& key, const QVariant& defaultValues) const;
    void setValue(const QString& key, const QVariant& value);
    void setValuewithGroup(const QString& settingsGroup, const QString& key, const QVariant& value);
signals:
    void settingsChanged(const QString& settingsGourp, const QString& key, const QVariant& value);
    void settingsChanged(const QString& key, const QVariant& value);
};

#endif // CONFIG_H
