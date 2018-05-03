#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    preprocessor = new Preprocessor();

    // Pad
    connect(preprocessor, &Preprocessor::axisLX, ui->slider_LX, &QSlider::setValue);
    connect(preprocessor, &Preprocessor::axisLY, ui->slider_LY, &QSlider::setValue);
    connect(preprocessor, &Preprocessor::axisRX, ui->slider_RX, &QSlider::setValue);
    connect(preprocessor, &Preprocessor::axisRY, ui->slider_RY, &QSlider::setValue);
    connect(preprocessor, &Preprocessor::axisL2, ui->slider_L2, &QSlider::setValue);
    connect(preprocessor, &Preprocessor::axisR2, ui->slider_R2, &QSlider::setValue);
    connect(preprocessor, &Preprocessor::connected, ui->gamepad_connected, &QCheckBox::setChecked);

    // Command
    connect(preprocessor, &Preprocessor::command_changed, this, &MainWindow::on_command_changed);
    connect(ui->scale_box_acc, SIGNAL(valueChanged(int)), preprocessor, SLOT(on_acc_scale_changed(int)));
    connect(ui->scale_box_pitch, SIGNAL(valueChanged(int)), preprocessor, SLOT(on_pitch_scale_changed(int)));


    // Charts
    ui->chart_ping->set_axis_names("time","ping");

    ui->chart_drone_hg->set_axis_names("time","command - H G");
    ui->chart_drone_hg->show_average(false);
    ui->chart_drone_hg->set_ranges(0, 10000, 0, 65535);

    ui->chart_drone_hd->set_axis_names("time","command - H D");
    ui->chart_drone_hd->show_average(false);
    ui->chart_drone_hd->set_ranges(0, 10000, 0, 65535);

    ui->chart_drone_bg->set_axis_names("time","command - B G");
    ui->chart_drone_bg->show_average(false);
    ui->chart_drone_bg->set_ranges(0, 10000, 0, 65535);

    ui->chart_drone_bd->set_axis_names("time","command - B D");
    ui->chart_drone_bd->show_average(false);
    ui->chart_drone_bd->set_ranges(0, 10000, 0, 65535);

}



// Connection
void MainWindow::on_drone_connection_clicked() {
    drone = new Transmitter;
    connect(drone, SIGNAL(connected()), this, SLOT(on_drone_connected()));
    connect(drone, SIGNAL(finished()), this, SLOT(on_drone_disconnected()));
    connect(drone, SIGNAL(ping_response(int)), this, SLOT(on_ping_received(int)));
    connect(preprocessor, &Preprocessor::command_changed, drone, &Transmitter::send);
    drone->start();
    drone->connect_to(ui->drone_spin_ip_1->value(), ui->drone_spin_ip_2->value());
}

void MainWindow::on_drone_connected() {
    ui->connexion_box->setEnabled(false);
}

void MainWindow::on_drone_disconnected() {
    delete drone;
    ui->connexion_box->setEnabled(true);
}


// UI interactions
void MainWindow::on_ping_received(int ms) {
    ui->chart_ping->append(ms);
}

void MainWindow::on_command_changed(Command cmd) {
    ui->chart_drone_hg->append(cmd.Values.motor_H_G);
    ui->chart_drone_hd->append(cmd.Values.motor_H_D);
    ui->chart_drone_bg->append(cmd.Values.motor_B_G);
    ui->chart_drone_bd->append(cmd.Values.motor_B_D);
}

MainWindow::~MainWindow() {
    delete ui;
}
