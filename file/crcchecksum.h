#ifndef CRCCHECKSUM_H
#define CRCCHECKSUM_H

#include <QByteArray>



class CrcChecksum
{
public:
    CrcChecksum()=delete;

    static quint16 crc16ForModbus(const QByteArray &data);
    static quint16 crc16ForX25(const QByteArray &data);

};

#endif // CRCCHECKSUM_H
