#include "preprocessor.h"

Preprocessor::Preprocessor() {
    pad = new QGamepad();

    _timer = new QTimer;
    _timer->setInterval(100);
    _timer->setSingleShot(true);
    connect(_timer, &QTimer::timeout, this, &Preprocessor::on_timer_timeout);

    cmd.Values.motor_B_D = 10;
    cmd.Values.motor_H_D = 10;
    cmd.Values.motor_H_G = 10;
    cmd.Values.motor_B_G = 10;

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
}

void Preprocessor::preprocess_speed() {
    _pitch = (double)map(pad->axisLeftX()*10000, -10000, 10000, 9000, 11000) / 10000.;
    _roll = (double)map(pad->axisRightX()*10000, -10000, 10000, 9000, 11000) / 10000.;
    _acc =(double)map((0.5+pad->buttonR2()/2-pad->buttonL2()/2)*10000, 0, 10000, 9000, 11000) / 10000.;

    if(!_timer->isActive())
        _timer->start();
}

void Preprocessor::on_timer_timeout() {
    unsigned long B_D = cmd.Values.motor_B_D*_acc;
    unsigned long H_D = cmd.Values.motor_H_D*_acc;
    unsigned long B_G = cmd.Values.motor_B_G*_acc;
    unsigned long H_G = cmd.Values.motor_H_G*_acc;

    qDebug() << "Acc = " << QString::number(_acc) << " - Before acc : " << QString::number(cmd.Values.motor_B_D) << " - After acc : " << QString::number(B_D);

    if(_pitch>1) {
        H_D*=_pitch;
        B_D*=_pitch;
    } else {
        H_G*=(2-_pitch);
        B_G*=(2-_pitch);
    }
    if(_roll>1) {
        B_G*=_roll;
        B_D*=_roll;
    } else {
        H_G*=(2-_roll);
        H_D*=(2-_roll);
    }

    //apply_filter(&B_D,&H_D,&B_G,&H_G);

    if(B_D>65535)
        B_D = 65535;
    else if(B_D<=10)
        B_D = 10;
    if(H_D>65535)
        H_D = 65535;
    else if(H_D<=10)
        H_D = 10;
    if(B_G>65535)
        B_G = 65535;
    else if(B_G<=10)
        B_G = 10;
    if(H_G>65535)
        H_G = 65535;
    else if(H_G<=10)
        H_G = 10;

    cmd.Values.motor_B_D = (unsigned short) B_D;
    cmd.Values.motor_B_G = (unsigned short) B_G;
    cmd.Values.motor_H_D = (unsigned short) H_D;
    cmd.Values.motor_H_G = (unsigned short) H_G;

    // qDebug() <<  "acc " << QString::number(_acc) <<  "pitch " << QString::number(_pitch) <<  "roll " << QString::number(_roll);
    qDebug() << " HG " << QString::number(cmd.Values.motor_H_G) << " HD " << QString::number(cmd.Values.motor_H_D)
             << " BG " << QString::number(cmd.Values.motor_B_G) << " BD " << QString::number(cmd.Values.motor_B_D);

    emit(command_changed(cmd));
}

void Preprocessor::apply_filter(unsigned long *B_D, unsigned long *H_D, unsigned long *B_G, unsigned long *H_G) {
    *H_G = *H_G + _x1.Values.motor_H_G/6 + _y1.Values.motor_H_G/3 + _y2.Values.motor_H_G/6;
    *H_D = *H_D + _x1.Values.motor_H_D/6 + _y1.Values.motor_H_D/3 + _y2.Values.motor_H_D/6;
    *B_D = *B_D + _x1.Values.motor_B_G/6 + _y1.Values.motor_B_G/3 + _y2.Values.motor_B_G/6;
    *B_G = *B_G + _x1.Values.motor_B_D/6 + _y1.Values.motor_B_D/3 + _y2.Values.motor_B_D/6;
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


int Preprocessor::map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
