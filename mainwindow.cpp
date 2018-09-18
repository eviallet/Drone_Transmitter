#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/images/ic_launcher.png"));
    setWindowTitle("Drone Controller");

    _pidparams.ykp = 0;
    _pidparams.ytd = 0;
    _pidparams.yti = 0;
    _pidparams.pkp = 0;
    _pidparams.pti = 0;
    _pidparams.ptd = 0;
    _pidparams.rkp = 0;
    _pidparams.rtd = 0;
    _pidparams.rti = 0;
    _pidparams.checksum = 0;


    ui->visualization->setVisible(false);

    preprocessor = new Preprocessor();
    preprocessor->start();

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
    connect(ui->double_spinbox_ykp, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));
    connect(ui->double_spinbox_yti, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));
    connect(ui->double_spinbox_ytd, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));

    connect(ui->double_spinbox_pkp, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));
    connect(ui->double_spinbox_pti, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));
    connect(ui->double_spinbox_ptd, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));

    connect(ui->double_spinbox_rkp, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));
    connect(ui->double_spinbox_rti, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));
    connect(ui->double_spinbox_rtd, SIGNAL(valueChanged(double)), this, SLOT(on_pid_changed(double)));

    // Motors settings
    connect(ui->settings_all, SIGNAL(toggled(bool)), this, SLOT(on_settings_changed(bool)));
    connect(ui->settings_pitch, SIGNAL(toggled(bool)), this, SLOT(on_settings_changed(bool)));
    connect(ui->settings_roll, SIGNAL(toggled(bool)), this, SLOT(on_settings_changed(bool)));
    connect(ui->settings_none, SIGNAL(toggled(bool)), this, SLOT(on_settings_changed(bool)));

    // Plot
    _yaw_plot = ui->drone_angle_graph_yaw;
    _yaw_plot->set_type(YAW);
    _pitch_plot = ui->drone_angle_graph_pitch;
    _pitch_plot->set_type(PITCH);
    _roll_plot = ui->drone_angle_graph_roll;
    _roll_plot->set_type(ROLL);

    // Transmitter
    transmitter = new Transmitter;
    connect(transmitter, SIGNAL(connected()), this, SLOT(on_transmitter_connected()));
    connect(transmitter, SIGNAL(disconnected()), this, SLOT(on_transmitter_disconnected()));
    connect(transmitter, &Transmitter::remote_sensor_infos, this, &MainWindow::on_remote_sensor_infos_received);
    connect(preprocessor, SIGNAL(command_changed(SetPoint)), transmitter, SLOT(send(SetPoint)));
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
    transmitter->connect_to(ui->drone_spin_ip_1->value(), ui->drone_spin_ip_2->value());
}

void MainWindow::on_transmitter_connected() {
    ui->connexion_box->setEnabled(false);
    ui->box_settings->setEnabled(true);
    ui->box_pid_y->setEnabled(true);
    ui->box_pid_p->setEnabled(true);
    ui->box_pid_r->setEnabled(true);
    ui->pid_save->setEnabled(true);
    ui->pid_load->setEnabled(true);
    ui->pid_reset->setEnabled(true);
    ui->log->setEnabled(true);
}

void MainWindow::on_transmitter_disconnected() {
    ui->connexion_box->setEnabled(true);
    ui->box_settings->setEnabled(false);
    ui->box_pid_y->setEnabled(false);
    ui->box_pid_p->setEnabled(false);
    ui->box_pid_r->setEnabled(false);
    ui->pid_save->setEnabled(false);
    ui->pid_load->setEnabled(false);
    ui->pid_reset->setEnabled(false);
    ui->log->setEnabled(false);
}





// Corrector
void MainWindow::on_pid_save_clicked() {
    QString filename = QFileDialog::getSaveFileName(this);
    QFile *save = new QFile(filename);
    save->open(QFile::WriteOnly);
    QString y_kp = QString::number(ui->double_spinbox_ykp->value());
    QString y_ti = QString::number(ui->double_spinbox_yti->value());
    QString y_td = QString::number(ui->double_spinbox_ytd->value());

    QString p_kp = QString::number(ui->double_spinbox_pkp->value());
    QString p_ti = QString::number(ui->double_spinbox_pti->value());
    QString p_td = QString::number(ui->double_spinbox_ptd->value());

    QString r_kp = QString::number(ui->double_spinbox_rkp->value());
    QString r_ti = QString::number(ui->double_spinbox_rti->value());
    QString r_td = QString::number(ui->double_spinbox_rtd->value());

    QTextStream(&*save) << y_kp << endl << y_ti << endl << y_td << endl <<
            p_kp << endl << p_ti << endl << p_td << endl <<
            r_kp << endl << r_ti << endl << r_td << endl;
    save->close();
}

void MainWindow::on_pid_load_clicked() {
    QString filename = QFileDialog::getOpenFileName(this);
    QFile *load = new QFile(filename);
    load->open(QFile::ReadOnly);

    QByteArray content = load->readAll();
    QString contentString(content);

    if(contentString.count('\n')==9)

    ui->double_spinbox_ykp->setValue(contentString.section('\n',0,0).toDouble());
    ui->double_spinbox_yti->setValue(contentString.section('\n',1,1).toDouble());
    ui->double_spinbox_ytd->setValue(contentString.section('\n',2,2).toDouble());

    ui->double_spinbox_pkp->setValue(contentString.section('\n',3,3).toDouble());
    ui->double_spinbox_pti->setValue(contentString.section('\n',4,4).toDouble());
    ui->double_spinbox_ptd->setValue(contentString.section('\n',5,5).toDouble());

    ui->double_spinbox_rkp->setValue(contentString.section('\n',6,6).toDouble());
    ui->double_spinbox_rti->setValue(contentString.section('\n',7,7).toDouble());
    ui->double_spinbox_rtd->setValue(contentString.section('\n',8,8).toDouble());
}

void MainWindow::on_pid_reset_clicked() {
    ui->double_spinbox_ykp->setValue(0);
    ui->double_spinbox_yti->setValue(0);
    ui->double_spinbox_ytd->setValue(0);

    ui->double_spinbox_pkp->setValue(0);
    ui->double_spinbox_pti->setValue(0);
    ui->double_spinbox_ptd->setValue(0);

    ui->double_spinbox_rkp->setValue(0);
    ui->double_spinbox_rti->setValue(0);
    ui->double_spinbox_rtd->setValue(0);
}


void MainWindow::on_pid_changed(double unused) {
    _pidparams.ykp = (float)ui->double_spinbox_ykp->value();
    _pidparams.yti = (float)ui->double_spinbox_ytd->value();
    _pidparams.ytd = (float)ui->double_spinbox_yti->value();

    _pidparams.pkp = (float)ui->double_spinbox_pkp->value();
    _pidparams.ptd = (float)ui->double_spinbox_pti->value();
    _pidparams.pti = (float)ui->double_spinbox_ptd->value();

    _pidparams.rkp = (float)ui->double_spinbox_rkp->value();
    _pidparams.rti = (float)ui->double_spinbox_rtd->value();
    _pidparams.rtd = (float)ui->double_spinbox_rti->value();

    transmitter->send(_pidparams);
}



// Settings
void MainWindow::on_settings_changed(bool checked) {
    if(checked) {
        if(ui->settings_all->isChecked())
            transmitter->send(get_settings(SETTINGS_SET::ALL));
        else if(ui->settings_pitch->isChecked())
            transmitter->send(get_settings(SETTINGS_SET::PITCH_TEST));
        else if(ui->settings_roll->isChecked())
            transmitter->send(get_settings(SETTINGS_SET::ROLL_TEST));
        else
            transmitter->send(get_settings(SETTINGS_SET::STOP));
    }
}



// 3D visualization
void MainWindow::on_visualization_checkbox_stateChanged(int checked) {
    bool c = checked==Qt::Checked;
    ui->drone_angle_graph_pitch->setVisible(!c);
    ui->drone_angle_graph_roll->setVisible(!c);
    ui->drone_angle_graph_yaw->setVisible(!c);
    ui->visualization->setVisible(c);
    ui->visualization->visualization_state(c);
}


// Other UI interactions
void MainWindow::on_remote_sensor_infos_received(SensorData s) {
    _yaw_plot->append(s);
    _pitch_plot->append(s);
    _roll_plot->append(s);

    ui->visualization->update_angles(s);

    if(_log&&_log_file->isOpen()) {
        if(_log_start==0)
            _log_start = QDateTime::currentMSecsSinceEpoch();
        QTextStream(&*_log_file) << QString("%1\t%2\t%3\t%4\n").arg(QDateTime::currentMSecsSinceEpoch()-_log_start).arg(s.yaw).arg(s.pitch).arg(s.roll);
    }
}

void MainWindow::on_log_stateChanged(int checked) {
    _log = checked==Qt::Checked;
    if(_log) {
        _log_file = new QFile(QFileDialog::getSaveFileName(this));
        _log_file->open(QFile::WriteOnly);
    } else if(_log_file->isOpen())
        _log_file->close();
}

MainWindow::~MainWindow() {
    if(_log_file!=Q_NULLPTR&&_log_file->isOpen())
            _log_file->close();
    delete ui;
}
