#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>


#include "realtimeplot.h"
#include "transmitter.h"
#include "preprocessor.h"

namespace Ui {
    class MainWindow;
}

using namespace QtCharts;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_drone_connection_clicked();
    void on_drone_connected();
    void on_drone_disconnected();
    void on_ping_received(int ms);
    void on_command_changed(Command cmd);
    void on_gamepad_event(bool connected);
    void on_left_stick_moved(int dx, int dy);
    void on_right_stick_moved(int dx, int dy);
    void on_left_shoulder_moved(int dy);
    void on_right_shoulder_moved(int dy);
    void on_y_pressed(bool pressed);
    void on_x_pressed(bool pressed);
    void on_b_pressed(bool pressed);
    void on_a_pressed(bool pressed);
private:
    Ui::MainWindow *ui;
    Transmitter *drone;
    Preprocessor *preprocessor;

    int _lx, _ly, _rx, _ry, _ls, _rs;
};

#endif // MAINWINDOW_H
