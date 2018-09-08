#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QDebug>
#include <QtMath>


#define SETPOINT_THRESHOLD 0.02

// OUT DATA

static char SETPOINT = 's';
struct SetPoint {
    unsigned short speed;
    short yaw;
    short pitch;
    short roll;
    short checksum;
};

static QByteArray encode_setpoint(SetPoint s) {
    s.checksum = s.speed+s.yaw+s.pitch+s.roll;
    QByteArray bytes;
    bytes.append(SETPOINT).append(SETPOINT)
            .append(reinterpret_cast<const char*>(&s.speed), sizeof(short))
            .append(reinterpret_cast<const char*>(&s.yaw), sizeof(short))
            .append(reinterpret_cast<const char*>(&s.pitch), sizeof(short))
            .append(reinterpret_cast<const char*>(&s.roll), sizeof(short))
            .append(reinterpret_cast<const char*>(&s.checksum), sizeof(short));
    //qDebug() << QString("encode_setpoint : %1 | %2 | %3 | %4").arg(s.speed).arg(s.yaw).arg(s.pitch).arg(s.roll);
    return bytes;
}

static bool is_equal(SetPoint &left, SetPoint &right) {
    return left.speed==right.speed && left.yaw==right.yaw &&
            left.pitch==right.pitch && left.roll==right.roll;
}

static bool compute_variation(SetPoint &left, SetPoint &right) {
    bool speed, yaw, pitch, roll;
    if(left.speed==0) {
        if(right.speed==0)
            speed=false;
        else
            speed=(qAbs(right.speed-left.speed)/right.speed)>SETPOINT_THRESHOLD;
    }
    if(left.yaw==0) {
        if(right.yaw==0)
            yaw=false;
        else
            yaw=(qAbs(right.yaw-left.yaw)/right.yaw)>SETPOINT_THRESHOLD;
    }
    if(left.pitch==0) {
        if(right.pitch==0)
            pitch=false;
        else
            pitch=(qAbs(right.pitch-left.pitch)/right.pitch)>SETPOINT_THRESHOLD;
    }
    if(left.roll==0) {
        if(right.roll==0)
            roll=false;
        else
            roll=(qAbs(right.roll-left.roll)/right.roll)>SETPOINT_THRESHOLD;
    }
    return speed || yaw || pitch || roll;
}





static char PIDPARAMS = 'p';
struct PIDParams {
    float ykp;
    float yti;
    float ytd;
    float pkp;
    float ptd;
    float pti;
    float rkp;
    float rtd;
    float rti;
    float checksum;
};

static QByteArray encode_pid_params(PIDParams p) {
    p.checksum = p.ykp+p.ytd+p.yti+p.pkp+p.ptd+p.pti+p.rkp+p.rtd+p.rti;
    QByteArray bytes;
    bytes.append(PIDPARAMS).append(PIDPARAMS)
            .append(reinterpret_cast<const char*>(&p.ykp), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.ytd), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.yti), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.pkp), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.ptd), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.pti), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.rkp), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.rtd), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.rti), sizeof(float))
            .append(reinterpret_cast<const char*>(&p.checksum), sizeof(float));
    return bytes;
}



// IN DATA

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
