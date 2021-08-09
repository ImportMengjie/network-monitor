#include "deviceinfo.h"
#include "file/pathutils.h"

const QString DeviceInfo::DeviceName = "设备名称";
const QString DeviceInfo::DeviceID = "设备ID";
const QString DeviceInfo::DeviceType = "设备类型";
const QString DeviceInfo::DeviceState = "设备状态";
const QString DeviceInfo::DeviceIco = "设备图标";

DeviceInfo::DeviceInfo() {

}

QString DeviceInfo::getDeviceName() {
    if(data.count(DeviceInfo::DeviceName))
        return data[DeviceInfo::DeviceName].value;
    else
        return "";
}

QString DeviceInfo::getDeviceId() {
    return data[DeviceInfo::DeviceID].value;
}

QString DeviceInfo::getDeviceType() {
    return data[DeviceInfo::DeviceType].value;
}

QString DeviceInfo::getDeviceState() {
    return data[DeviceInfo::DeviceState].value;
}

QString DeviceInfo::getDeviceIco()
{
    return PathUtils::getAbsPath(data[DeviceInfo::DeviceIco].value);
}
