#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <QObject>
#include <QGamepad>
#include <QDebug>
#include <QTime>
#include <QTimer>

#include "packet.h"

#define LOW_LIMIT 0
#define HIGH_LIMIT 65535
#define FACTOR 40


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
    void x_button(bool);
    void b_button(bool);
    void a_button(bool);
    void connected(bool);
    void command_changed(Command);
public slots:
    void on_acc_scale_changed(int value);
    void on_pitch_scale_changed(int value);
    void on_offsets_changed(int,int,int,int);
    void on_offsets_enabled(bool);
private slots:
    void on_timer_timeout();
    void on_y_button_pressed(bool);
    void on_x_button_pressed(bool);
    void on_b_button_pressed(bool);
    void on_a_button_pressed(bool);
    void on_axisLeftXChanged(double value);
    void on_axisLeftYChanged(double value);
    void on_axisRightXChanged(double value);
    void on_axisRightYChanged(double value);
    void on_buttonL2Changed(double value);
    void on_buttonR2Changed(double value);
    void on_connectedChanged(bool value);
private:
    static int map(int,int,int,int,int);
private:
    QGamepad *pad;
    Command uniform_cmd;
    QTimer *_timer;
    double _roll = 0 , _pitch = 0, _acc = 0;

    int _acc_scale = 10;
    int _pitch_scale = 10;

    int _offset_hg = 0, _offset_hd = 0, _offset_bg = 0, _offset_bd = 0;
    bool _offsets_enabled = false;
};

#endif // PREPROCESSOR_H
