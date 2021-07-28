#ifndef RUNNINGSTATE_H
#define RUNNINGSTATE_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QtEndian>

struct DeviceState{
    quint8 deviceID;
    quint8 runningStatus;
    QByteArray status;

    QString getDeviceID(){
        return QString("%1").arg(deviceID);
    }

    QString getRunningStatus(){
        return QString("%1").arg(runningStatus, 0, 16);
    }

    QString getSatus(){
        QString ret = "";
        for(int i=0;i<status.size();i++){
            quint8 num = qFromBigEndian<quint8>(status.mid(i, 1));
            ret+=QString("%1").arg(num, 2, 16, QLatin1Char('0'));
        }
        return ret;
    }

};

class RunningState : public QObject
{
    Q_OBJECT
public:
    DeviceState centerDevice;
    QVector<DeviceState> othersDevice;
    QString reportTime;
    QDateTime reportDateTime;

    const static quint16 magic;
    const static quint8 goodStatus;
    const static quint8 badStatus;
    static bool loadeFromFile(const QString& filePath, RunningState& runningState);

    static QByteArray readAndMove(const QByteArray& data, int& left, int count);

    QVector<DeviceState> getAllDevice(){
        return othersDevice + QVector<DeviceState>({centerDevice});
    }


    explicit RunningState();

signals:

private:

};

#endif // RUNNINGSTATE_H
