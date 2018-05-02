#include "transmitter.h"

Transmitter::Transmitter() {
    // Ping
    _ping_timer = new QTimer;
    _ping_timer->setInterval(PING_MAX_DELAY_MS);
    _ping_timer->setSingleShot(true);
    connect(_ping_timer, SIGNAL(timeout()), this, SLOT(on_ping_timeout()));
    connect(this, SIGNAL(start_ping()), this, SLOT(ping()));

    // Ping socket
    _socket_ping = new QTcpSocket;
    connect(_socket_ping, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_socket_ping_error(QAbstractSocket::SocketError)));
    connect(_socket_ping, SIGNAL(connected()), this, SLOT(on_socket_ping_connected()));
    connect(_socket_ping, SIGNAL(readyRead()), this, SLOT(on_socket_ping_readyRead()));

    // Data socket
    _socket_data = new QTcpSocket;
    connect(_socket_data, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_socket_data_error(QAbstractSocket::SocketError)));
    connect(_socket_data, SIGNAL(connected()), this, SLOT(on_socket_data_connected()));
    connect(_socket_data, SIGNAL(readyRead()), this, SLOT(on_socket_data_readyRead()));
}

void Transmitter::run() {
    while(_waiting_for_connection);

    while(_is_connected) {
        if((QTime::currentTime().msecsSinceStartOfDay()-_last_ping_millis >= PING_DELAY_MS)&&_last_ping_finished) {
            emit(start_ping());
            _last_ping_millis = QTime::currentTime().msecsSinceStartOfDay();
        }
    }
}

void Transmitter::connect_to(int address1, int address2) {
    qDebug() << "Connecting to host";
    _socket_data->connectToHost(QHostAddress(QString("192.168.%1.%2").arg(address1).arg(address2)), 50000);
    _socket_ping->connectToHost(QHostAddress(QString("192.168.%1.%2").arg(address1).arg(address2)), 50001);
}





void Transmitter::on_socket_data_connected() {
    qDebug() << "Data connected";
    emit(connected());
}

void Transmitter::on_socket_data_error(QAbstractSocket::SocketError e) {
    emit(error());
    qWarning() << "Data - Socket error :" << e;
}

void Transmitter::on_socket_data_readyRead() {
    qDebug() << "Data - Received : " << QString(_socket_data->readAll());
}

void Transmitter::send(Command cmd) {
    _socket_data->write(cmd.Bytes);
}





void Transmitter::on_socket_ping_connected() {
    qDebug() << "Ping connected";
    _is_connected = true;
    _waiting_for_connection = false;
}

void Transmitter::on_socket_ping_error(QAbstractSocket::SocketError e) {
    qDebug() << "Ping - Socket error:" << e;
}

void Transmitter::on_socket_ping_readyRead() {
    _socket_ping->readAll();
    _last_ping_finished = true;
    emit(ping_response(PING_MAX_DELAY_MS-_ping_timer->remainingTime()));
    _ping_timer->stop();
}

void Transmitter::ping() {
    _last_ping_finished = false;
    _ping_timer->start();
    _socket_ping->write("0");
}

void Transmitter::on_ping_timeout() {
    qDebug() << "Ping timed out";
}
