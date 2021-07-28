#include <QCoreApplication>
#include <QByteArray>
#include <QDebug>
#include <QVector>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <unistd.h>

QByteArray &operator<<(QByteArray &l, quint8 r) {
    l.append(r);
    return l;
}

QByteArray &operator<<(QByteArray &l, quint16 r) {
    return l<<quint8(r>>8)<<quint8(r);
}

QByteArray &operator<<(QByteArray &l, quint32 r) {
    return l<<quint16(r>>16)<<quint16(r);
}

QByteArray get_state_array(int state, quint8 id){
    QByteArray stateArray;
    QByteArray stateData;
    stateData<<id;
    if(state)
        stateData<<quint8(0x6A);
    else
        stateData<<quint8(0x40);
    stateData<<quint32(0);
    stateArray<<quint8(stateData.size());
    stateArray.append(stateData);
    return stateArray;
}

void generate_file(const quint8& center_id,const QVector<quint8>& ids, int state){
    auto current = QDateTime::currentDateTime();
    quint16 magic = 0x3AB9;
    QString output_path = QString("/Users/mengjie/Documents/project/55/folder/%1.amg")
            .arg(current.toString("yyyy_MM_dd_hh_mm_ss_zzz"));
    QFile file(output_path);
    file.open(QIODevice::WriteOnly);
    QByteArray frontArray;
    frontArray<<magic
              <<quint16(1)
              <<quint8(current.date().year()-2000)
              <<quint8(current.date().month())
              <<quint8(current.date().day())
              <<quint8(current.time().hour())
              <<quint8(current.time().minute())
              <<quint8(current.time().second())
              <<quint16(current.time().msec());
    QByteArray dataArray;
    QByteArray centerDataArray;
    centerDataArray<<center_id<<quint8(0);
    if(state)
        centerDataArray<<quint8(0x6A);
    else
        centerDataArray<<quint8(0x40);
    centerDataArray<<quint32(0)<<quint16(31)<<quint16(41)<<quint16(51)<<quint8(0);
    dataArray<<quint8(centerDataArray.size());
    dataArray.append(centerDataArray);
    for(auto& id:ids){
        dataArray.append(get_state_array(state, id));
    }
    frontArray<<quint32(dataArray.size());
    frontArray.append(dataArray);
    quint16 checkSumValue = qChecksum(frontArray, frontArray.size());
    qDebug()<<checkSumValue<<endl;
    frontArray<<checkSumValue;
    file.write(frontArray);
    file.close();
}

int main(int argc, char *argv[]) {
    QVector<quint8> ids{11,12,31,32,33,34,35,36,51,52,53,54,55,56};
    int i = 0;
    while(true){
        qDebug()<<"output"<<endl;
        generate_file(1,ids, i);
        i = 1-i;
        sleep(10);
    }

    return 0;
}
