#include "transmitter.h"

Transmitter::Transmitter() {
    // Data socket
    _socket_data = new QTcpSocket;
    connect(_socket_data, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_socket_data_error(QAbstractSocket::SocketError)));
    connect(_socket_data, SIGNAL(disconnected()), this, SLOT(on_socket_disconnected()));
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

void Transmitter::send(Command cmd) {
    if(_socket_data->state()==QAbstractSocket::ConnectedState && !is_equal(cmd,_last_command)) {
        _socket_data->write(QByteArray::fromRawData(cmd.Bytes, 4*sizeof(short)).data());
        _last_command = cmd;
    }
}

// TODO read input Sensor objeect
void Transmitter::on_socket_data_readyRead() {
    while((_socket_data->bytesAvailable()%(4*sizeof(short))==0)&&_socket_data->bytesAvailable()!=0) {
        QByteArray received = _socket_data->read(4*sizeof(short));
        /*
        Command cmd;
        memcpy(cmd.Bytes, received.data(), 4*sizeof(short));
        qDebug() << "Received : HG " << QString::number(cmd.speed) << " HD " << QString::number(cmd.yaw)
                 << " BG " << QString::number(cmd.pitch) << " BD " << QString::number(cmd.roll);
        emit(remote_sensor_infos(cmd));*/
    }

    if(_socket_data->bytesAvailable()>0)
        _socket_data->readAll();
}
