#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>
#include <QMap>
#include <QXmlStreamAttributes>

struct Value{
    QString value = "";
    QString max_value = "";
    QString min_value = "";
    bool readDatabase = false;
    QString mask = "";
    QString key = "";

    bool loadFromAttributes(const QXmlStreamAttributes& attributes){

        if(attributes.hasAttribute("name")){
            this->key = attributes.value("name").toString();
        }else	return false;
        if(attributes.hasAttribute("readDatabase")){
            this->readDatabase = attributes.value("readDatabase").toInt();
        }
        if(attributes.hasAttribute("value")){
            this->value = attributes.value("value").toString();
        }
        if(this->value==""&&!this->readDatabase)
            return false;
        if(attributes.hasAttribute("maxValue")){
            this->max_value = attributes.value("maxValue").toInt();
        }
        if(attributes.hasAttribute("minValue")){
            this->min_value = attributes.value("minValue").toInt();
        }
        if(attributes.hasAttribute("mask")){
            this->mask = attributes.value("mask").toString();
        }
        return true;
    }

};

class DeviceInfo
{
public:
    static QString DeviceName;
    static QString DeviceID;
    static QString DeviceType;
    static QString DeviceState;
    static QString DeviceIco;
    DeviceInfo();

    QMap<QString, Value> data;

    void addValue(const Value& v){
        data[v.key] = v;
    }

    QString getDeviceName();
    QString getDeviceId();
    QString getDeviceType();
    QString getDeivceState();

};

#endif // DEVICEINFO_H
