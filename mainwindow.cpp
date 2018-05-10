#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/ic_launcher.png"));
    setWindowTitle("Drone Controller");
    showFullScreen();


    preprocessor = new Preprocessor();

    // Pad
    connect(preprocessor, &Preprocessor::left_stick, this, &MainWindow::on_left_stick_moved);
    connect(preprocessor, &Preprocessor::right_stick, this, &MainWindow::on_right_stick_moved);
    connect(preprocessor, &Preprocessor::left_shoulder, this, &MainWindow::on_left_shoulder_moved);
    connect(preprocessor, &Preprocessor::right_shoulder, this, &MainWindow::on_right_shoulder_moved);
    connect(preprocessor, &Preprocessor::connected, this, &MainWindow::on_gamepad_event);
    connect(preprocessor, &Preprocessor::y_button, this, &MainWindow::on_y_pressed);

    _lx = ui->left_stick_icon->x();
    _ly = ui->left_stick_icon->y();
    _rx = ui->right_stick_icon->x();
    _ry = ui->right_stick_icon->y();

    _ls = ui->left_shoulder->y();
    _rs = ui->right_shoulder->y();

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

// Gamepad
void MainWindow::on_gamepad_event(bool connected) {
    if(connected)
        ui->gamepad_connected->setPixmap(QPixmap::fromImage(QImage(":/buttons/btn_connected")));
    else
        ui->gamepad_connected->setPixmap(QPixmap::fromImage(QImage(":/buttons/btn_disconnected")));
}

void MainWindow::on_y_pressed(bool pressed) {
    if(pressed)
        ui->xbox_y->setPixmap(QPixmap::fromImage(QImage(":/icons/ic_y_pressed")));
    else
        ui->xbox_y->setPixmap(QPixmap::fromImage(QImage(":/icons/ic_y")));
}

void MainWindow::on_left_stick_moved(int dx, int dy) {
    ui->left_stick_icon->move(_lx+dx, _ly+dy);
}

void MainWindow::on_right_stick_moved(int dx, int dy) {
    ui->right_stick_icon->move(_rx+dx, _ry+dy);
}

void MainWindow::on_left_shoulder_moved(int dy) {
    ui->left_shoulder->move(ui->left_shoulder->x(), _ls+dy);
}

void MainWindow::on_right_shoulder_moved(int dy) {
    ui->right_shoulder->move(ui->right_shoulder->x(), _rs+dy);
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
    ui->chart_ping->clear_data();
    drone->terminate();
    delete drone;
    ui->connexion_box->setEnabled(true);
}


// UI interactions
void MainWindow::on_ping_received(int ms) {
    ui->chart_ping->append(ms);
}

void MainWindow::on_command_changed(Command cmd) {
    ui->chart_drone_hg->append(cmd.motor_H_G);
    ui->chart_drone_hd->append(cmd.motor_H_D);
    ui->chart_drone_bg->append(cmd.motor_B_G);
    ui->chart_drone_bd->append(cmd.motor_B_D);
}

MainWindow::~MainWindow() {
    delete ui;
}
