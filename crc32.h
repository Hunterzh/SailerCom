#ifndef CRC32_H
#define CRC32_H

#include <QtGlobal>
#include <QByteArray>

class crc32
{
public:
    crc32();

    static QByteArray GetCRC32(QByteArray data);
    static QByteArray GetCRC32(QByteArray data, int start, int count);
    static QByteArray intToByte(int i);


private:
    static quint32 crcTable[];
};

#endif // CRC32_H
