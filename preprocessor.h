#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <QObject>
#include <QGamepad>
#include <QDebug>
#include <QTime>
#include <QTimer>

#include "packet.h"


class Preprocessor : public QObject {
    Q_OBJECT
public:
    Preprocessor();
    void preprocess_speed();
signals:
    void axisLX(int);
    void axisLY(int);
    void axisRX(int);
    void axisRY(int);
    void axisL2(int);
    void axisR2(int);
    void connected(bool);
    void command_changed(Command);
public slots:
    void on_axisLeftXChanged(double value);
    void on_axisLeftYChanged(double value);
    void on_axisRightXChanged(double value);
    void on_axisRightYChanged(double value);
    void on_buttonL2Changed(double value);
    void on_buttonR2Changed(double value);
    void on_connectedChanged(bool value);
private slots:
    void on_timer_timeout();
    void apply_filter(unsigned long *,unsigned long *,unsigned long *,unsigned long *);
private:
    static int map(int,int,int,int,int);
private:
    QGamepad *pad;
    Command cmd;
    QTimer *_timer;
    double _roll = 1, _pitch = 1, _acc = 1;


    Command _x1;
    Command _y1;
    Command _y2;
};

#endif // PREPROCESSOR_H
