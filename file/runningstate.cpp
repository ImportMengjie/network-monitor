#include "runningstate.h"
#include "file/crcchecksum.h"

#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QtEndian>

const quint16 RunningState::magic= 0x3AB9;
const quint8 RunningState::goodStatus = 0x6A;
const quint8 RunningState::badStatus = 0x40;

template<class T>
    static T conver(const QByteArray& array){
    return qFromBigEndian<T>(array);
}

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
    bool isok=false;
    quint16 crc = conver<quint16>(totalData.right(2));
    QByteArray data = totalData.left(totalData.size()-2);
    if(CrcChecksum::crc16ForX25(data)!=crc){
        qWarning()<<filePath<<" crc test error"<<endl;
        return false;
    }
    int left=0;
    if(conver<quint16>(readAndMove(data, left, 2))!=magic){
        qWarning()<<filePath<<" magic number error "<<endl;
        return false;
    }
    left+=2;
    uint year,month = 0, day=0, hours=0, minus=0,sec=0,millisec=0;
    year = conver<quint8>(readAndMove(data, left, 1)) + 2000;
    month = conver<quint8>(readAndMove(data,left,1));
    day = conver<quint8>(readAndMove(data,left,1));
    hours = conver<quint8>(readAndMove(data, left, 1));
    minus = conver<quint8>(readAndMove(data, left, 1));
    sec = conver<quint8>(readAndMove(data, left,1));
    millisec = conver<quint16>(readAndMove(data,left,2));
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


    uint total_data_size =  conver<quint32>(readAndMove(data, left, 4));
    if(total_data_size!=data.size()-left){
        qWarning()<<"file size:"<<data.size()-left<<";need size:"<<total_data_size<<endl;
        return false;
    }
    {
        uint status_len = conver<quint8>(readAndMove(data, left, 1));
        QByteArray status_bytes = readAndMove(data, left, status_len);
        runningState.centerDevice.deviceID = conver<quint8>(status_bytes.left(1));
        runningState.centerDevice.runningStatus = conver<quint8>(status_bytes.mid(2, 1));
        if(runningState.centerDevice.runningStatus!=goodStatus&&runningState.centerDevice.runningStatus!=badStatus){
            qWarning()<<"error center device status:"<<runningState.centerDevice.runningStatus<<endl;
            return false;
        }
        runningState.centerDevice.status = status_bytes;
    }
    while(left<data.size()){
        DeviceState device;
        uint status_len = conver<quint8>(readAndMove(data, left, 1));
        QByteArray status_bytes = readAndMove(data, left, status_len);
        device.deviceID = conver<quint8>(status_bytes.left(1));
        device.runningStatus = conver<quint8>(status_bytes.mid(1, 1));
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
