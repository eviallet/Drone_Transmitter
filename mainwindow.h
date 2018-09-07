#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

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

    void on_ykp_changed(double value);
    void on_yti_changed(double value);
    void on_ytd_changed(double value);

    void on_pkp_changed(double value);
    void on_pti_changed(double value);
    void on_ptd_changed(double value);

    void on_rkp_changed(double value);
    void on_rti_changed(double value);
    void on_rtd_changed(double value);
private:
    Ui::MainWindow *ui;
    Transmitter *transmitter;
    Preprocessor *preprocessor;
    RealTimePlot *_yaw_plot, *_pitch_plot, *_roll_plot;

    int _lx, _ly, _rx, _ry, _ls, _rs;
};

#endif // MAINWINDOW_H
