#ifndef CONFIG_H
#define CONFIG_H
#include "common.h"
class Config
{
private:
    void initConfig(const QString& filePath);
public:
    void readConfig();
    Config();
};

#endif // CONFIG_H
