#include "transmitter.h"

Transmitter::Transmitter() {
    // Data socket
    _socket_data = new QTcpSocket;
    connect(_socket_data, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_socket_data_error(QAbstractSocket::SocketError)));
    connect(_socket_data, SIGNAL(connected()), this, SLOT(on_socket_data_connected()));
    connect(_socket_data, SIGNAL(disconnected()), this, SLOT(on_socket_data_connected()));
    connect(_socket_data, SIGNAL(readyRead()), this, SLOT(on_socket_data_readyRead()));
}


void Transmitter::connect_to(int address1, int address2) {
    qDebug() << "Connecting to host";
    _socket_data->connectToHost(QHostAddress(QString("192.168.%1.%2").arg(address1).arg(address2)), 50000);
}



void Transmitter::on_socket_data_connected() {
    qDebug() << "Data connected";
    emit(connected());
    _is_connected = true;
    _waiting_for_connection = false;
}

void Transmitter::on_socket_disconnected() {
    _is_connected = false;
    emit(disconnected());
}

void Transmitter::on_socket_data_error(QAbstractSocket::SocketError e) {
    qWarning() << "Data - Socket error :" << e;
}

void Transmitter::send(SetPoint sp) {
    if(_socket_data->state()==QAbstractSocket::ConnectedState) {
        _socket_data->write(encode_setpoint(sp));
        _last_sp = sp;
    }
}

void Transmitter::send(PIDParams p) {
    if(_socket_data->state()==QAbstractSocket::ConnectedState) {
        _socket_data->write(encode_pid_params(p));
    }
}

void Transmitter::send(Settings t) {
    if(_socket_data->state()==QAbstractSocket::ConnectedState) {
        _socket_data->write(encode_settings(t));
    }
}

void Transmitter::on_socket_data_readyRead() {
    QByteArray received = _socket_data->readAll();
    while(received.length()>0) {
        //qDebug() << QString("Reading %1 bytes.").arg(received.length());
        for(int i=0; i<received.length(); i++) {
            SensorData s = sensor_from_bytes(received.left(4*sizeof(float)));
            emit(remote_sensor_infos(s));
            received.remove(0,4*sizeof(float));
        }
    }
}
