#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
#include <QTime>

#include "packet.h"

class Transmitter : public QObject {
    Q_OBJECT
public:
    Transmitter();
signals:
    void connected();
    void disconnected();
    void remote_sensor_infos(SensorData);
public slots:
    void connect_to(int address, int address2);
    void send(PIDParams);
    void send_setpoint(SetPoint);
private slots:
    void on_socket_data_connected();
    void on_socket_data_error(QAbstractSocket::SocketError error);
    void on_socket_data_readyRead();
    void on_socket_disconnected();
private:
    QTcpSocket *_socket_data;
    bool _waiting_for_connection = true;
    bool _is_connected = false;
    SetPoint _last_sp;
};

#endif // TRANSMITTER_H
