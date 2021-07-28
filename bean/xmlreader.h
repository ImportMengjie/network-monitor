#ifndef XMLREADER_H
#define XMLREADER_H

#include <QString>
#include <QVector>

#include "baseconfig.h"
#include "deviceinfo.h"
#include "file/pathutils.h"

class XmlReader
{
public:
    QString filePath;

    BaseConfig baseconfig;

    QVector<DeviceInfo> sendDeviceList;
    QVector<DeviceInfo> receiveDeviceList;

    DeviceInfo centerDevice;

    DeviceInfo sendBridgeDevice;
    DeviceInfo receiveBridgeDevice;


    int useBridge = true;

    XmlReader(QString filePath);

    bool readFromXml();

    QString verfiy(){
        if(baseconfig.dbDriver.isEmpty()||baseconfig.dbHost.isEmpty()||baseconfig.dbName.isEmpty()||baseconfig.dbPort<=0||baseconfig.dbUserName.isEmpty())
            return "数据库配置有问题！";
        if(baseconfig.errorDir.isEmpty()||baseconfig.historyDir.isEmpty()||baseconfig.monitorDir.isEmpty()||baseconfig.outputDir.isEmpty())
            return "错误、历史、监控、输出目录必须设置！";
        if(!(PathUtils::isDirExists(baseconfig.errorDir)&&PathUtils::isDirExists(baseconfig.historyDir)&&PathUtils::isDirExists(baseconfig.monitorDir)&&PathUtils::isDirExists(baseconfig.outputDir)))
            return "错误、历史、监控、输出目录必须存在";
        if(baseconfig.refreshInterval<=0||baseconfig.reportInterval<=0)
            return "刷新间隔和报告间隔必须大于0";
        if(baseconfig.sytemId.isEmpty())
            return "sytemId 必须不为空";
        if(baseconfig.monitorDir==baseconfig.errorDir)
            return "监控目录和错误目录必须不一样";

        return "";
    }

    bool isDeviceGood(const QString& deviceId, const QString& content, const QString& runningState) const{
        return true;
    }

};

#endif // XMLREADER_H
