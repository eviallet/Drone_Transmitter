#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QDebug>

struct Command {
    union {
        char Bytes[8];
        struct {
            unsigned short speed;
            unsigned short yaw;
            unsigned short pitch;
            unsigned short roll;
        };
    };
};

bool is_equal(Command &left, Command &right);


struct SensorData {
    float alt;
    float yaw;
    float pitch;
    float roll;
};

static SensorData sensor_from_bytes(QByteArray bytes) {
    SensorData s;
    s.alt = *reinterpret_cast<const float*>(bytes.left(4).data());
    s.yaw = *reinterpret_cast<const float*>(bytes.mid(4,4).data());
    s.pitch = *reinterpret_cast<const float*>(bytes.mid(8,4).data());
    s.roll = *reinterpret_cast<const float*>(bytes.right(4).data());
    //qDebug() << QString("Alt = %1 - Yaw = %2 - Pitch = %3 - Roll = %4").arg(s.alt).arg(s.yaw).arg(s.pitch).arg(s.roll);
    return s;
}

#endif // PACKET_H
