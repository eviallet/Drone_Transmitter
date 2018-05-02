#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QTime>

#include "packet.h"

#define PING_DELAY_MS 100
#define PING_MAX_DELAY_MS 250

class Transmitter : public QThread {
    Q_OBJECT
public:
    Transmitter();
    void run() override;
signals:
    void connected();
    void error();
    void ping_response(int ms);
    void disconnected();
    void start_ping();
public slots:
    void connect_to(int address1, int address2);
    void send(Command cmd);
private slots:
    void ping();
    void on_ping_timeout();
    void on_socket_ping_connected();
    void on_socket_ping_error(QAbstractSocket::SocketError error);
    void on_socket_ping_readyRead();

    void on_socket_data_connected();
    void on_socket_data_error(QAbstractSocket::SocketError error);
    void on_socket_data_readyRead();
private:
    QTimer *_ping_timer;
    QTcpSocket *_socket_data;
    QTcpSocket *_socket_ping;
    bool _waiting_for_connection = true;
    bool _is_connected = false;
    int _last_ping_millis = 0;
    bool _last_ping_finished = true;
};

#endif // TRANSMITTER_H
