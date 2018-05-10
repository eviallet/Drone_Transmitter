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
signals:
    void left_stick(int,int);
    void right_stick(int,int);
    void left_shoulder(int);
    void right_shoulder(int);
    void y_button(bool);
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

    void on_acc_scale_changed(int value);
    void on_pitch_scale_changed(int value);
private slots:
    void on_timer_timeout();
    void on_y_button_pressed(bool);
private:
    static int map(int,int,int,int,int);
private:
    QGamepad *pad;
    Command cmd;
    QTimer *_timer;
    double _roll = 0 , _roll_prev = 0, _pitch = 0,_pitch_prev = 0, _acc = 0;

    int _acc_scale = 10;
    int _pitch_scale = 7500;
};

#endif // PREPROCESSOR_H
