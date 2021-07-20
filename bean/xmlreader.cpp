#include "xmlreader.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QXmlStreamReader>

XmlReader::XmlReader(QString filePath):filePath(filePath) {
}

bool XmlReader::readFromXml() {
    QFile file(filePath);
    QXmlStreamReader reader;
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return false;

    reader.setDevice(&file);
    while(!reader.atEnd()){
        if(reader.isStartElement()){
            if(reader.name()=="常规") {
                /*
                "系统ID"   ""
                "系统名称"   ""
                "监视文件夹"   ""
                "历史文件夹"   ""
                "错误文件夹"   ""
                "导出文件夹"   ""
                "刷新显示间隔秒数"   ""
                "状态上报间隔秒数"   ""
                */
                while(!(reader.name()=="常规"&&reader.isEndElement())){
                    if(reader.isStartElement()){
                        if(reader.name()=="系统ID"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.sytemId = reader.text().toString();
                            }else
                                return false;
                        } else if(reader.name()=="系统名称"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.sytemName = reader.text().toString();
                            }else
                                return false;
                        } else if (reader.name()=="监视文件夹"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.monitorDir = reader.text().toString();
                            }else
                                return false;
                        } else if(reader.name()=="历史文件夹"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.historyDir = reader.text().toString();
                            }else
                                return false;
                        } else if(reader.name()=="错误文件夹"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.errorDir = reader.text().toString();
                            }else
                                return false;
                        } else if(reader.name()=="导出文件夹"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.outputDir = reader.text().toString();
                            }else
                                return false;
                        } else if(reader.name()=="刷新显示间隔秒数"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.refreshInterval = reader.text().toUInt();
                            }else
                                return false;
                        } else if(reader.name()=="状态上报间隔秒数"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.reportInterval = reader.text().toUInt();
                            }else
                                return false;
                        } else if(reader.name()=="数据库host"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.dbHost = reader.text().toString();
                            }else
                                return false;
                        } else if(reader.name()=="数据库dbname"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.dbName = reader.text().toString();
                            }else
                                return false;
                        } else if(reader.name()=="数据库username"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.dbUserName = reader.text().toString();
                            }else
                                return false;
                        } else if(reader.name()=="数据库password"){
                            if(reader.readNext()==reader.Characters){
                                baseconfig.dbPassword = reader.text().toString();
                            }else
                                return false;
                        }
                    }
                    reader.readNext();
                }
            } else if(reader.name()=="中控"){
                while(!(reader.name()=="中控"&&reader.isEndElement())){
                    if(reader.isStartElement() && reader.name()=="property"){
                        Value v;
                        if(!v.loadFromAttributes(reader.attributes()))
                            return false;
                        centerDevice.addValue(v);
                    }
                    reader.readNext();
                }
            } else if(reader.name()=="中继"){
                while(!(reader.name()=="中继"&&reader.isEndElement())){
                    if(reader.isStartElement() && reader.name()=="配备"){
                        if(reader.readNext()==reader.Characters){
                        useBridge = reader.text().toInt();
                        }else
                        return false;
                    }
                    else if(reader.isStartElement() && reader.name()=="发中继"){
                        while(!(reader.name()=="发中继"&&reader.isEndElement())){
                        if(reader.isStartElement() && reader.name()=="property"){
                            Value v;
                            if(!v.loadFromAttributes(reader.attributes()))
                            return false;
                            sendBridgeDevice.addValue(v);
                        }
                        reader.readNext();
                        }
                    } else if(reader.isStartElement() && reader.name()=="收中继"){
                        while(!(reader.name()=="收中继"&&reader.isEndElement())){
                        if(reader.isStartElement() && reader.name()=="property"){
                            Value v;
                            if(!v.loadFromAttributes(reader.attributes()))
                            return false;
                            receiveBridgeDevice.addValue(v);
                        }
                        reader.readNext();
                        }

                    }
                    reader.readNext();
                }
            } else if(reader.name()=="接入"){
                while(!(reader.name()=="接入"&&reader.isEndElement())){
                    if(reader.name()=="接入设备"){
                        DeviceInfo deviceInfo;
                        while(!(reader.name()=="接入设备"&&reader.isEndElement())){
                            if(reader.isStartElement() && reader.name()=="property"){
                                Value v;
                                if(!v.loadFromAttributes(reader.attributes()))
                                    return false;
                                deviceInfo.addValue(v);
                            }
                            reader.readNext();
                        }
                        this->sendDeviceList.append(deviceInfo);
                    }
                    reader.readNext();
                }
            } else if(reader.name()=="转发"){
                while(!(reader.name()=="转发"&&reader.isEndElement())){
                    if(reader.name()=="转发设备"){
                        DeviceInfo deviceInfo;
                        while(!(reader.name()=="转发设备"&&reader.isEndElement())){
                            if(reader.isStartElement() && reader.name()=="property"){
                                Value v;
                                if(!v.loadFromAttributes(reader.attributes()))
                                    return false;
                                deviceInfo.addValue(v);
                            }
                            reader.readNext();
                        }
                        this->receiveDeviceList.append(deviceInfo);
                    }
                    reader.readNext();
                }

            }
        }
        reader.readNext();
    }
    return true;
}
