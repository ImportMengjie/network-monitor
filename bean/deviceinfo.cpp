#include "deviceinfo.h"

QString DeviceInfo::DeviceName = "设备名称";
QString DeviceInfo::DeviceID = "设备ID";
QString DeviceInfo::DeviceType = "设备类型";
QString DeviceInfo::DeviceState = "设备状态";
QString DeviceInfo::DeviceIco = "设备图标";

DeviceInfo::DeviceInfo() {

}

QString DeviceInfo::getDeviceName() {
    return data[DeviceInfo::DeviceName].value;
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
    return data[DeviceInfo::DeviceIco].value;
}
