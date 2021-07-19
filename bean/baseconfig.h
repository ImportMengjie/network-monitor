#ifndef BASECONFIG_H
#define BASECONFIG_H

#include <QString>

class BaseConfig
{
public:
    BaseConfig();

    QString sytemId;
    QString sytemName;
    QString monitorDir;
    QString historyDir;
    QString errorDir;

    QString outputDir;
    uint refreshInterval;
    uint reportInterval;

};

#endif // BASECONFIG_H
