#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>

#include "transmitter.h"
#include "preprocessor.h"
#include "realtimeplot.h"

namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_transmitter_connection_clicked();
    void on_transmitter_connected();
    void on_transmitter_disconnected();
    void on_gamepad_event(bool connected);
    void on_left_stick_moved(int dx, int dy);
    void on_right_stick_moved(int dx, int dy);
    void on_left_shoulder_moved(int dy);
    void on_right_shoulder_moved(int dy);
    void on_y_pressed(bool pressed);
    void on_x_pressed(bool pressed);
    void on_b_pressed(bool pressed);
    void on_a_pressed(bool pressed);
    void on_remote_sensor_infos_received(SensorData s);

    void on_pid_changed(double unused);
    void on_pid_load_clicked();
    void on_pid_save_clicked();
    void on_pid_reset_clicked();
    void on_log_stateChanged(int checked);
    void on_settings_changed(bool checked);
    void on_visualization_checkbox_stateChanged(int checked);
private:
    Ui::MainWindow *ui;
    Transmitter *transmitter;
    Preprocessor *preprocessor;
    RealTimePlot *_yaw_plot, *_pitch_plot, *_roll_plot;
    PIDParams _pidparams;
    bool _log = false;
    QFile *_log_file = Q_NULLPTR;
    qint64 _log_start = 0;

    int _lx, _ly, _rx, _ry, _ls, _rs;
};

#endif // MAINWINDOW_H
