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

    QString dbDriver;
    QString dbHost;
    QString dbName;
    QString dbUserName;
    QString dbPassword;
    int dbPort;
};

#endif // BASECONFIG_H
