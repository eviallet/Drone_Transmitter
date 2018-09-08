#include "preprocessor.h"

Preprocessor::Preprocessor() {
    pad = new QGamepad();

    _last_sp.speed = LOW_LIMIT;
    _last_sp.yaw = LOW_LIMIT;
    _last_sp.pitch = LOW_LIMIT;
    _last_sp.roll = LOW_LIMIT;

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


    connect(this, &Preprocessor::compute_command, this, &Preprocessor::on_compute_command);
}


void Preprocessor::run() {
    while(true) {
        if(_last == 0)
            _last = QDateTime::currentMSecsSinceEpoch();
        else if(QDateTime::currentMSecsSinceEpoch() - _last >= 70) {
            emit(compute_command());
            _last = QDateTime::currentMSecsSinceEpoch();
        }
    }
}

void Preprocessor::on_compute_command() {
    _roll = (double)-pad->axisLeftX()*100;
    _pitch = (double)-pad->axisLeftY()*100;
    _acc = (pad->buttonR2()-pad->buttonL2()) * 100;

    long speed = _last_sp.speed + _acc*_acc_scale;

    if(speed>HIGH_LIMIT)
        speed = HIGH_LIMIT;
    else if(speed<=LOW_LIMIT)
        speed = LOW_LIMIT;


    SetPoint sp;
    sp.speed = (unsigned short) speed;
    sp.yaw = 0; // TODO using right stick angle?
    sp.pitch = (short) _pitch;
    sp.roll = (short) _roll;

    if(!is_equal(_last_sp,sp)&&compute_variation(sp,_last_sp)) {
        emit(command_changed(sp));
        _last_sp = sp;
    }
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
    _last_sp.speed = 0;
    _last_sp.yaw = 0;
    _last_sp.pitch = 0;
    _last_sp.roll = 0;
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



// https://www.arduino.cc/reference/en/language/functions/math/map/
int Preprocessor::map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
