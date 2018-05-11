#include "preprocessor.h"

Preprocessor::Preprocessor() {
    pad = new QGamepad();

    _timer = new QTimer;
    _timer->setInterval(50);
    _timer->setSingleShot(false);
    connect(_timer, &QTimer::timeout, this, &Preprocessor::on_timer_timeout);

    uniform_cmd.motor_B_D = LOW_LIMIT;
    uniform_cmd.motor_H_D = LOW_LIMIT;
    uniform_cmd.motor_H_G = LOW_LIMIT;
    uniform_cmd.motor_B_G = LOW_LIMIT;

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
    connect(pad, SIGNAL(buttonYChanged(bool)), this, SLOT(on_y_button_pressed(bool)));
    connect(pad, SIGNAL(buttonXChanged(bool)), this, SLOT(on_x_button_pressed(bool)));
    connect(pad, SIGNAL(buttonBChanged(bool)), this, SLOT(on_b_button_pressed(bool)));
    connect(pad, SIGNAL(buttonAChanged(bool)), this, SLOT(on_a_button_pressed(bool)));


    _timer->start();
}


void Preprocessor::on_timer_timeout() {
    _roll = (double)-pad->axisLeftX()*100;
    _pitch = (double)-pad->axisLeftY()*100;
    _acc = (pad->buttonR2()-pad->buttonL2()) * 100;


    long H_G = uniform_cmd.motor_H_G + _acc*_acc_scale;
    long H_D = uniform_cmd.motor_H_D + _acc*_acc_scale;
    long B_G = uniform_cmd.motor_B_G + _acc*_acc_scale;
    long B_D = uniform_cmd.motor_B_D + _acc*_acc_scale;

    if(H_D>HIGH_LIMIT)
        H_D = HIGH_LIMIT;
    else if(H_D<=LOW_LIMIT)
        H_D = LOW_LIMIT;

    if(H_G>HIGH_LIMIT)
        H_G = HIGH_LIMIT;
    else if(H_G<=LOW_LIMIT)
        H_G = LOW_LIMIT;

    if(B_G>HIGH_LIMIT)
        B_G = HIGH_LIMIT;
    else if(B_G<=LOW_LIMIT)
        B_G = LOW_LIMIT;

    if(B_D>HIGH_LIMIT)
        B_D = HIGH_LIMIT;
    else if(B_D<=LOW_LIMIT)
        B_D = LOW_LIMIT;

    uniform_cmd.motor_H_G = (unsigned short) H_G;
    uniform_cmd.motor_H_D = (unsigned short) H_D;
    uniform_cmd.motor_B_G = (unsigned short) B_G;
    uniform_cmd.motor_B_D = (unsigned short) B_D;


    // + +
    // + +
    if(_offsets_enabled) {
        H_G += _offset_hg;
        H_D += _offset_hd;
        B_G += _offset_bg;
        B_D += _offset_bd;
    }

    // ^
    // |
    // v
    H_G-=_pitch*_pitch_scale;
    H_D-=_pitch*_pitch_scale;
    B_G+=_pitch*_pitch_scale;
    B_D+=_pitch*_pitch_scale;

    // <->
    H_G-=_roll*_pitch_scale;
    H_D+=_roll*_pitch_scale;
    B_G-=_roll*_pitch_scale;
    B_D+=_roll*_pitch_scale;

    if(H_D>HIGH_LIMIT)
        H_D = HIGH_LIMIT;
    else if(H_D<=LOW_LIMIT)
        H_D = LOW_LIMIT;

    if(H_G>HIGH_LIMIT)
        H_G = HIGH_LIMIT;
    else if(H_G<=LOW_LIMIT)
        H_G = LOW_LIMIT;

    if(B_G>HIGH_LIMIT)
        B_G = HIGH_LIMIT;
    else if(B_G<=LOW_LIMIT)
        B_G = LOW_LIMIT;

    if(B_D>HIGH_LIMIT)
        B_D = HIGH_LIMIT;
    else if(B_D<=LOW_LIMIT)
        B_D = LOW_LIMIT;


    Command cmd;
    cmd.motor_H_G = (unsigned short) H_G;
    cmd.motor_H_D = (unsigned short) H_D;
    cmd.motor_B_G = (unsigned short) B_G;
    cmd.motor_B_D = (unsigned short) B_D;

    //qDebug() <<  "acc " << QString::number(_acc) <<  "pitch " << QString::number_pitch <<  "roll " << QString::number_roll);
    //qDebug() << " HG " << QString::number(cmd.motor_H_G) << " HD " << QString::number(cmd.motor_H_D)
    //         << " BG " << QString::number(cmd.motor_B_G) << " BD " << QString::number(cmd.motor_B_D);

    emit(command_changed(cmd));
}


// Pad joysticks
void Preprocessor::on_axisLeftXChanged(double value) {
    emit(left_stick(value*FACTOR, pad->axisLeftY()*FACTOR));
}
void Preprocessor::on_axisLeftYChanged(double value) {
    emit(left_stick(pad->axisLeftX()*FACTOR,value*FACTOR));
}
void Preprocessor::on_axisRightXChanged(double value) {
    emit(right_stick(value*FACTOR, pad->axisRightY()*FACTOR));
}
void Preprocessor::on_axisRightYChanged(double value) {
    emit(right_stick(pad->axisRightX()*FACTOR,value*FACTOR));
}

// Pad shoulders
void Preprocessor::on_buttonL2Changed(double value) {
    emit(left_shoulder(-value*FACTOR*2));
}
void Preprocessor::on_buttonR2Changed(double value) {
    emit(right_shoulder(-value*FACTOR*2));
}

// Pad events
void Preprocessor::on_connectedChanged(bool value) {
    emit(connected(value));
}

void Preprocessor::on_y_button_pressed(bool value) {
    uniform_cmd.motor_B_D = 0;
    uniform_cmd.motor_B_G = 0;
    uniform_cmd.motor_H_D = 0;
    uniform_cmd.motor_H_G = 0;
    emit(y_button(value));
}
void Preprocessor::on_x_button_pressed(bool value) {
    if(value)
        _acc_scale*=3;
    else
        _acc_scale/=3;
    emit(x_button(value));
}
void Preprocessor::on_b_button_pressed(bool value) {
    if(value)
        _acc_scale/=2;
    else
        _acc_scale*=2;
    emit(b_button(value));
}
void Preprocessor::on_a_button_pressed(bool value) {
    if(value)
        _acc_scale*=2;
    else
        _acc_scale/=2;
    emit(a_button(value));
}

// Tweaks
void Preprocessor::on_acc_scale_changed(int value) {
    _acc_scale = value;
}

void Preprocessor::on_pitch_scale_changed(int value) {
    _pitch_scale = value;
}

void Preprocessor::on_offsets_changed(int hg, int hd, int bg, int bd) {
    _offset_hg = hg;
    _offset_hd = hd;
    _offset_bg = bg;
    _offset_bd = bd;
}

void Preprocessor::on_offsets_enabled(bool enabled) {
    _offsets_enabled = enabled;
}



// Literally copy/pasted from https://www.arduino.cc/reference/en/language/functions/math/map/
int Preprocessor::map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
