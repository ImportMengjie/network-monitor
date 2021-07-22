#include "runningstate.h"
#include "file/crcchecksum.h"

#include <QFile>
#include <QDebug>
#include <QDataStream>

const quint16 RunningState::magic= 0x3AB9;
const quint8 RunningState::goodStatus = 0x6A;
const quint8 RunningState::badStatus = 0x40;

bool RunningState::loadeFromFile(const QString& filePath, RunningState& runningState)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)){
        qWarning()<<"error open file:"<<filePath<<endl;
        return false;
    }
    QByteArray totalData = file.readAll();
    file.close();
    if(totalData.size()<18){
        qWarning()<<filePath<<" file size is "<<totalData.size()<<endl;
        return false;
    }
    quint16 crc = totalData.right(2).toShort();
    QByteArray data = totalData.left(totalData.size()-2);
    if(CrcChecksum::crc16ForX25(data)!=crc){
        qWarning()<<filePath<<" crc test error"<<endl;
        return false;
    }
    int left=0;
    if(readAndMove(data, left, 2).toUShort()!=magic){
        qWarning()<<filePath<<" magic number error "<<endl;
        return false;
    }
    left+=2;
    uint year,month = 0, day=0, hours=0, minus=0,sec=0,millisec=0;
    year = readAndMove(data, left, 1).toUInt() + 2000;
    month = readAndMove(data,left,1).toUInt();
    day = readAndMove(data,left,1).toUInt();
    hours = readAndMove(data, left, 1).toUInt();
    minus = readAndMove(data, left, 1).toUInt();
    sec = readAndMove(data, left,1).toUInt();
    millisec = readAndMove(data,left,2).toUInt();
    runningState.reportDateTime.setDate(QDate(year, month, day));
    runningState.reportDateTime.setTime(QTime(hours, minus, sec, millisec));
    runningState.reportTime = QString("%1-%2-%3 %4:%5:%6:%7")
            .arg(year)
            .arg(month, 2, 10, QLatin1Char('0'))
            .arg(day, 2, 10, QLatin1Char('0'))
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minus, 2, 10, QLatin1Char('0'))
            .arg(sec, 2, 10, QLatin1Char('0'))
            .arg(millisec, 3, 10, QLatin1Char('0'));


    uint total_data_size =  readAndMove(data, left, 4).toUInt();
    if(total_data_size!=data.size()-left){
        qWarning()<<"file size:"<<data.size()-left<<";need size:"<<total_data_size<<endl;
        return false;
    }
    {
        uint status_len = readAndMove(data, left, 1).toUInt();
        QByteArray status_bytes = readAndMove(data, left, status_len);
        runningState.centerDevice.deviceID = status_bytes.left(1).toUInt();
        runningState.centerDevice.runningStatus = status_bytes.mid(2, 1).toUInt();
        if(runningState.centerDevice.runningStatus!=goodStatus&&runningState.centerDevice.runningStatus!=badStatus){
            qWarning()<<"error center device status:"<<runningState.centerDevice.runningStatus<<endl;
            return false;
        }
        runningState.centerDevice.status = status_bytes;
    }
    while(left<data.size()){
        DeviceState device;
        uint status_len = readAndMove(data, left, 1).toUInt();
        QByteArray status_bytes = readAndMove(data, left, status_len);
        device.deviceID = status_bytes.left(1).toUInt();
        device.runningStatus = status_bytes.mid(1, 1).toUInt();
        if(device.runningStatus!=goodStatus&&device.runningStatus!=badStatus){
            qWarning()<<"error other device status:"<<device.runningStatus<<endl;
            return false;
        }
        device.status = status_bytes;
        runningState.othersDevice.append(device);
    }
    return true;
}

QByteArray RunningState::readAndMove(const QByteArray &data, int &left, int count) {
    QByteArray ret = data.mid(left, count);
    left+=count;
    return ret;
}

RunningState::RunningState() : QObject(nullptr) {

}
