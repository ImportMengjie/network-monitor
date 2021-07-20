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
    QString unit = "";
    double multiple = 1;

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
        if(attributes.hasAttribute("unit")){
            this->unit = attributes.value("unit").toString();
        }
        if(attributes.hasAttribute("multiple")){
            this->multiple = attributes.value("multiple").toDouble();
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
    QVector<Value> otherList;
    int deviceState = 3;

    void addValue(const Value& v){
        data[v.key] = v;
        if(v.key!=DeviceName&&v.key!=DeviceID&&v.key!=DeviceType&&v.key!=DeviceState&&v.key!=DeviceIco)
            otherList.append(v);
    }

    QString getDeviceName();
    QString getDeviceId();
    QString getDeviceType();
    QString getDeviceState();
    QString getDeviceIco();

};

#endif // DEVICEINFO_H
