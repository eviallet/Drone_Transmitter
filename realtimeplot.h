#ifndef REALTIMEPLOT_H
#define REALTIMEPLOT_H

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QTime>
#include <QTimer>
#include <QDebug>

#include "packet.h"

using namespace QtCharts;

class RealTimePlot : public QChartView {
    Q_OBJECT
public:
    RealTimePlot(QWidget* parent = 0);
public slots:
    void append(int y, int x = QTime::currentTime().msecsSinceStartOfDay());
    void set_axis_names(QString x, QString y);
    void show_average(bool show);
    void set_ranges(int xmin, int xmax, int ymin, int ymax);
private:
    QSplineSeries *_series;
    QLineSeries *_series_average;
    int _x_min = 0;
    int _x_max = 10000;
    int _y_min = 0;
    int _y_max = 10;
    int _t_0 = 0;
    bool _show_average = true;
    QString _title_y;
};

#endif // REALTIMEPLOT_H
