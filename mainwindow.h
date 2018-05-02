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
private:
    Ui::MainWindow *ui;
    Transmitter *drone;
    Preprocessor *preprocessor;
};

#endif // MAINWINDOW_H
