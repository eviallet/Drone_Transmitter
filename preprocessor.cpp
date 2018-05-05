#include "preprocessor.h"

Preprocessor::Preprocessor() {
    pad = new QGamepad();

    _timer = new QTimer;
    _timer->setInterval(50);
    _timer->setSingleShot(false);
    connect(_timer, &QTimer::timeout, this, &Preprocessor::on_timer_timeout);

    cmd.motor_B_D = 0;
    cmd.motor_H_D = 0;
    cmd.motor_H_G = 0;
    cmd.motor_B_G = 0;

    // Pad joysticks
    connect(pad, SIGNAL(axisLeftXChanged(double)), this, SLOT(on_axisLeftXChanged(double)));
    connect(pad, SIGNAL(axisLeftYChanged(double)), this, SLOT(on_axisLeftYChanged(double)));
    connect(pad, SIGNAL(axisRightXChanged(double)), this, SLOT(on_axisRightXChanged(double)));
    connect(pad, SIGNAL(axisRightYChanged(double)), this, SLOT(on_axisRightYChanged(double)));
    // Pad shoulders
    connect(pad, SIGNAL(buttonL2Changed(double)), this, SLOT(on_buttonL2Changed(double)));
    connect(pad, SIGNAL(buttonR2Changed(double)), this, SLOT(on_buttonR2Changed(double)));
    // Pad events
    connect(pad, SIGNAL(connectedChanged(bool)), this, SLOT(on_connectedChanged(bool)));


    _timer->start();
}

void Preprocessor::preprocess_speed() {
    _pitch = (double)-pad->axisLeftX();
    _roll = (double)-pad->axisLeftY();
    _acc = (pad->buttonR2()-pad->buttonL2()) * 100;
}

void Preprocessor::on_timer_timeout() {
    long B_D = cmd.motor_B_D + _acc*_acc_scale;
    long H_D = cmd.motor_H_D + _acc*_acc_scale;
    long B_G = cmd.motor_B_G + _acc*_acc_scale;
    long H_G = cmd.motor_H_G + _acc*_acc_scale;

    H_D+=(_pitch-_pitch_prev)*_pitch_scale;
    B_D+=(_pitch-_pitch_prev)*_pitch_scale;
    H_G-=(_pitch-_pitch_prev)*_pitch_scale;
    B_G-=(_pitch-_pitch_prev)*_pitch_scale;

    _pitch_prev = _pitch;

    B_G+=(_roll-_roll_prev)*_pitch_scale;
    B_D+=(_roll-_roll_prev)*_pitch_scale;
    H_G-=(_roll-_roll_prev)*_pitch_scale;
    H_D-=(_roll-_roll_prev)*_pitch_scale;

    _roll_prev = _roll;

    if(B_D>65535)
        B_D = 65535;
    else if(B_D<=0)
        B_D = 0;

    if(H_D>65535)
        H_D = 65535;
    else if(H_D<=0)
        H_D = 0;

    if(B_G>65535)
        B_G = 65535;
    else if(B_G<=0)
        B_G = 0;

    if(H_G>65535)
        H_G = 65535;
    else if(H_G<=0)
        H_G = 0;

    cmd.motor_B_D = (unsigned short) B_D;
    cmd.motor_B_G = (unsigned short) B_G;
    cmd.motor_H_D = (unsigned short) H_D;
    cmd.motor_H_G = (unsigned short) H_G;

    //qDebug() <<  "acc " << QString::number(_acc) <<  "pitch " << QString::number(_pitch) <<  "roll " << QString::number(_roll);
    //qDebug() << " HG " << QString::number(cmd.motor_H_G) << " HD " << QString::number(cmd.motor_H_D)
    //         << " BG " << QString::number(cmd.motor_B_G) << " BD " << QString::number(cmd.motor_B_D);

    emit(command_changed(cmd));
}


// Pad joysticks
void Preprocessor::on_axisLeftXChanged(double value) {
    value*=100;
    preprocess_speed();
    emit(axisLX(value));
}
void Preprocessor::on_axisLeftYChanged(double value) {
    value = -value*100;
    preprocess_speed();
    emit(axisLY(value));
}
void Preprocessor::on_axisRightXChanged(double value) {
    value*=100;
    preprocess_speed();
    emit(axisRX(value));
}
void Preprocessor::on_axisRightYChanged(double value) {
    value = -value*100;
    preprocess_speed();
    emit(axisRY(value));
}

// Pad shoulders
void Preprocessor::on_buttonL2Changed(double value) {
    value*=100;
    preprocess_speed();
    emit(axisL2(value));
}
void Preprocessor::on_buttonR2Changed(double value) {
    value*=100;
    preprocess_speed();
    emit(axisR2(value));
}

// Pad events
void Preprocessor::on_connectedChanged(bool value) {
    emit(connected(value));
}

void Preprocessor::on_acc_scale_changed(int value) {
    _acc_scale = value;
}

void Preprocessor::on_pitch_scale_changed(int value) {
    _pitch_scale = value;
}


int Preprocessor::map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
