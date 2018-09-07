#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/images/ic_launcher.png"));
    setWindowTitle("Drone Controller");


    preprocessor = new Preprocessor();

    // Pad
    connect(preprocessor, &Preprocessor::left_stick, this, &MainWindow::on_left_stick_moved);
    connect(preprocessor, &Preprocessor::right_stick, this, &MainWindow::on_right_stick_moved);
    connect(preprocessor, &Preprocessor::left_shoulder, this, &MainWindow::on_left_shoulder_moved);
    connect(preprocessor, &Preprocessor::right_shoulder, this, &MainWindow::on_right_shoulder_moved);
    connect(preprocessor, &Preprocessor::connected, this, &MainWindow::on_gamepad_event);
    connect(preprocessor, &Preprocessor::y_button, this, &MainWindow::on_y_pressed);
    connect(preprocessor, &Preprocessor::x_button, this, &MainWindow::on_x_pressed);
    connect(preprocessor, &Preprocessor::b_button, this, &MainWindow::on_b_pressed);
    connect(preprocessor, &Preprocessor::a_button, this, &MainWindow::on_a_pressed);

    _lx = ui->left_stick_icon->x();
    _ly = ui->left_stick_icon->y();
    _rx = ui->right_stick_icon->x();
    _ry = ui->right_stick_icon->y();

    _ls = ui->left_shoulder->y();
    _rs = ui->right_shoulder->y();

    // Corrector control
    connect(ui->double_spinbox_ykp, SIGNAL(valueChanged(double)), this, SLOT(on_ykp_changed(double)));
    connect(ui->double_spinbox_yti, SIGNAL(valueChanged(double)), this, SLOT(on_yti_changed(double)));
    connect(ui->double_spinbox_ytd, SIGNAL(valueChanged(double)), this, SLOT(on_ytd_changed(double)));

    connect(ui->double_spinbox_pkp, SIGNAL(valueChanged(double)), this, SLOT(on_pkp_changed(double)));
    connect(ui->double_spinbox_pti, SIGNAL(valueChanged(double)), this, SLOT(on_pti_changed(double)));
    connect(ui->double_spinbox_ptd, SIGNAL(valueChanged(double)), this, SLOT(on_ptd_changed(double)));

    connect(ui->double_spinbox_rkp, SIGNAL(valueChanged(double)), this, SLOT(on_rkp_changed(double)));
    connect(ui->double_spinbox_rti, SIGNAL(valueChanged(double)), this, SLOT(on_rti_changed(double)));
    connect(ui->double_spinbox_rtd, SIGNAL(valueChanged(double)), this, SLOT(on_rtd_changed(double)));

    // Plot
    _yaw_plot = ui->drone_angle_graph_yaw;
    _yaw_plot->set_type(YAW);
    _pitch_plot = ui->drone_angle_graph_pitch;
    _pitch_plot->set_type(PITCH);
    _roll_plot = ui->drone_angle_graph_roll;
    _roll_plot->set_type(ROLL);
}

// Gamepad
void MainWindow::on_gamepad_event(bool connected) {
    if(connected)
        ui->gamepad_connected->setPixmap(QPixmap::fromImage(QImage(":/icons/images/btn_connected.png")));
    else
        ui->gamepad_connected->setPixmap(QPixmap::fromImage(QImage(":/icons/images/btn_disconnected.png")));
}

// Gamepad buttons
void MainWindow::on_y_pressed(bool pressed) {
    if(pressed)
        ui->xbox_y->setPixmap(QPixmap::fromImage(QImage(":/buttons/images/ic_y_pressed.png")));
    else
        ui->xbox_y->setPixmap(QPixmap::fromImage(QImage(":/buttons/images/ic_y.png")));
}
void MainWindow::on_x_pressed(bool pressed) {
    if(pressed)
        ui->xbox_x->setPixmap(QPixmap::fromImage(QImage(":/buttons/images/ic_x_pressed.png")));
    else
        ui->xbox_x->setPixmap(QPixmap::fromImage(QImage(":/buttons/images/ic_x.png")));
}
void MainWindow::on_b_pressed(bool pressed) {
    if(pressed)
        ui->xbox_b->setPixmap(QPixmap::fromImage(QImage(":/buttons/images/ic_b_pressed.png")));
    else
        ui->xbox_b->setPixmap(QPixmap::fromImage(QImage(":/buttons/images/ic_b.png")));
}
void MainWindow::on_a_pressed(bool pressed) {
    if(pressed)
        ui->xbox_a->setPixmap(QPixmap::fromImage(QImage(":/buttons/images/ic_a_pressed.png")));
    else
        ui->xbox_a->setPixmap(QPixmap::fromImage(QImage(":/buttons/images/ic_a.png")));
}

// Gamepad sticks
void MainWindow::on_left_stick_moved(int dx, int dy) {
    ui->left_stick_icon->move(_lx+dx, _ly+dy);
}

void MainWindow::on_right_stick_moved(int dx, int dy) {
    ui->right_stick_icon->move(_rx+dx, _ry+dy);
}

// Gamepad shoulders
void MainWindow::on_left_shoulder_moved(int dy) {
    ui->left_shoulder->move(ui->left_shoulder->x(), _ls+dy);
}

void MainWindow::on_right_shoulder_moved(int dy) {
    ui->right_shoulder->move(ui->right_shoulder->x(), _rs+dy);
}




// Connection
void MainWindow::on_transmitter_connection_clicked() {
    transmitter = new Transmitter;
    connect(transmitter, SIGNAL(connected()), this, SLOT(on_transmitter_connected()));
    connect(transmitter, SIGNAL(disconnected()), this, SLOT(on_transmitter_disconnected()));
    connect(transmitter, &Transmitter::remote_sensor_infos, this, &MainWindow::on_remote_sensor_infos_received);
    connect(preprocessor, &Preprocessor::command_changed, transmitter, &Transmitter::send);
    transmitter->connect_to(ui->drone_spin_ip_1->value(), ui->drone_spin_ip_2->value());
}

void MainWindow::on_transmitter_connected() {
    ui->connexion_box->setEnabled(false);
}

void MainWindow::on_transmitter_disconnected() {
    delete transmitter;
    ui->connexion_box->setEnabled(true);
}





// Corrector
// Yaw
void MainWindow::on_ykp_changed(double value) {

}

void MainWindow::on_yti_changed(double value) {

}

void MainWindow::on_ytd_changed(double value) {

}

// Pitch
void MainWindow::on_pkp_changed(double value) {

}

void MainWindow::on_pti_changed(double value) {

}

void MainWindow::on_ptd_changed(double value) {

}

// Roll
void MainWindow::on_rkp_changed(double value) {

}

void MainWindow::on_rti_changed(double value) {

}

void MainWindow::on_rtd_changed(double value) {

}





// UI interactions
void MainWindow::on_remote_sensor_infos_received(SensorData s) {
    _yaw_plot->append(s);
    _pitch_plot->append(s);
    _roll_plot->append(s);
}

MainWindow::~MainWindow() {
    delete ui;
}
