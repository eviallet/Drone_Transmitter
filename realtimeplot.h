#ifndef REALTIMEPLOT_H
#define REALTIMEPLOT_H

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QTime>
#include <QTimer>
#include <QDebug>

#include "packet.h"

enum AYPR{
    ALT,
    YAW,
    PITCH,
    ROLL
};

using namespace QtCharts;

class RealTimePlot : public QChartView {
    Q_OBJECT
public:
    RealTimePlot(QWidget* parent = 0);
public slots:
    void append(SensorData x);
    void append(int y, int x = QTime::currentTime().msecsSinceStartOfDay());
    void set_axis_names(QString x, QString y);
    void set_ranges(int xmin, int xmax, int ymin, int ymax);
    void clear_data();
    void set_type(AYPR type);
private:
    QSplineSeries *_series;
    QLineSeries *_series_average;
    int _x_min = 0;
    int _x_max = 10000;
    int _y_min = -90;
    int _y_max = 90;
    int _t_0 = 0;
    QString _title_y;
    AYPR _type;
};

#endif // REALTIMEPLOT_H
