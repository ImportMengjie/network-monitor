#ifndef XMLREADER_H
#define XMLREADER_H

#include <QString>
#include <QVector>

#include "baseconfig.h"
#include "deviceinfo.h"

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

};

#endif // XMLREADER_H
