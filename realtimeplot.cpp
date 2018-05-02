#include "realtimeplot.h"

RealTimePlot::RealTimePlot(QWidget* parent) : QChartView(parent) {
    _series = new QSplineSeries;
    _series_average = new QLineSeries;

    chart()->addSeries(_series);
    chart()->addSeries(_series_average);
    chart()->setAnimationOptions(QChart::SeriesAnimations);
    setRenderHint(QPainter::Antialiasing);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(_x_min, _x_max);
    chart()->axisY()->setRange(_y_min, _y_max);
    chart()->legend()->hide();
}


void RealTimePlot::set_ranges(int xmin, int xmax, int ymin, int ymax) {
    _x_min = xmin;
    _x_max = xmax;
    _y_min = ymin;
    _y_max = ymax;
    chart()->axisX()->setRange(_x_min, _x_max);
    chart()->axisY()->setRange(_y_min, _y_max);
}

void RealTimePlot::set_axis_names(QString x, QString y) {
    _title_y = y;
    chart()->axisX()->setTitleText(x);
    chart()->axisY()->setTitleText(y);
}

void RealTimePlot::show_average(bool show) {
    _show_average = show;
}


void RealTimePlot::set_update_time(int msec) {
    _timed = true;
    _timer = new QTimer;
    _timer->setInterval(msec);
    _timer->setSingleShot(false);
    connect(_timer, &QTimer::timeout, this, &RealTimePlot::on_timer_timeout);
}

void RealTimePlot::on_timer_timeout() {
    append(_series->at(_series->points().size()-1).y());
}


void RealTimePlot::append(int y, int x) {
    if(_timed)
        _timer->stop();

    if(_t_0==0)
        _t_0 = x;
    x-=_t_0;

    _series->append(x, y);


    if(x >= _x_max - 100) {
        int dx = x - _x_max + 100;
        _x_max += dx;
        _x_min += dx;
        chart()->axisX()->setRange(_x_min, _x_max);
    }

    if(y >= _y_max - 10) {
        int dy = y - _y_max + 10;
        _y_max += dy;
        chart()->axisY()->setRange(_y_min, _y_max);
    }

    if(_show_average) {

        int sum = 0, count = 0;
        for(int i=0; i < _series->points().size(); i++) {
            if(_x_min <= _series->points().at(i).x() && _series->points().at(i).x() <= _x_max) {
                sum += _series->points().at(i).y();
                count++;
            }
        }


        sum/=count;

        _series_average->clear();
        _series_average->append(_x_min, sum);
        _series_average->append(_x_max, sum);

        chart()->axisY()->setTitleText(_title_y+" "+QString::number(sum)+" ms");
        if(sum<=20)
            chart()->axisY()->setTitleBrush(QBrush(QColor(59, 214, 38)));
        else if(sum <=50)
            chart()->axisY()->setTitleBrush(QBrush(QColor(118, 237, 83)));
        else if(sum <=75)
            chart()->axisY()->setTitleBrush(QBrush(QColor(197, 239, 26)));
        else if(sum <=100)
            chart()->axisY()->setTitleBrush(QBrush(QColor(255, 174, 0)));
        else
            chart()->axisY()->setTitleBrush(QBrush(QColor(232, 27, 27)));
    }

    if(_timed)
        _timer->start();
}


