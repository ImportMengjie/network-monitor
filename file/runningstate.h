#ifndef RUNNINGSTATE_H
#define RUNNINGSTATE_H

#include <QObject>
#include <QVector>
#include <QDateTime>

struct DeviceState{
    quint8 deviceID;
    quint8 runningStatus;
    QByteArray status;

};

class RunningState : public QObject
{
    Q_OBJECT
public:
    DeviceState centerDevice;
    QVector<DeviceState> othersDevice;
    QString reportTime;

    const static quint16 magic;
    const static quint8 goodStatus;
    const static quint8 badStatus;
    static bool loadeFromFile(const QString& filePath, RunningState& runningState);

    static QByteArray readAndMove(const QByteArray& data, int& left, int count);


    explicit RunningState();

signals:

private:

};

#endif // RUNNINGSTATE_H
