#include "realtimeplot.h"

RealTimePlot::RealTimePlot(QWidget* parent) : QChartView(parent) {
    _series = new QSplineSeries;
    _series->setUseOpenGL(true);
    //_series_average = new QLineSeries;

    chart()->addSeries(_series);
    //chart()->addSeries(_series_average);
    chart()->setAnimationOptions(QChart::NoAnimation);
    setRenderHint(QPainter::Antialiasing);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(_x_min, _x_max);
    chart()->axisY()->setRange(_y_min, _y_max);
    chart()->legend()->hide();
    chart()->setTheme(QChart::ChartThemeBlueIcy);

    // Important to avoid our graph to look weird when optimizing memory usage later
    disconnect(_series, SIGNAL(pointRemoved(int)), this, SLOT(update()));
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

void RealTimePlot::set_type(AYPR type) {
    _type = type;
    QPen pen = _series->pen();
    switch(type) {
    case ALT:
        chart()->setTitle("Altitude");
        pen.setColor(QColor::fromRgb(101, 66, 244));
        break;
    case YAW:
        chart()->setTitle("Yaw");
        pen.setColor(QColor::fromRgb(56, 165, 89));
        break;
    case PITCH:
        chart()->setTitle("Pitch");
        pen.setColor(QColor::fromRgb(181, 95, 30));
        break;
    case ROLL:
        chart()->setTitle("Roll");
        pen.setColor(QColor::fromRgb(44, 107, 163));
        break;
    }
    _series->setPen(pen);
}

void RealTimePlot::append(SensorData x) {
    switch(_type) {
    case ALT:
        append(x.alt);
        break;
    case YAW:
        append(x.yaw);
        break;
    case PITCH:
        append(x.pitch);
        break;
    case ROLL:
        append(x.roll);
        break;
    }
}


void RealTimePlot::append(int y, int x) {
    if(_t_0==0)
        _t_0 = x;
    x-=_t_0;

    _series->append(x, y);


    // Move the graph with the data
    if(x >= _x_max - 100) {
        int dx = x - _x_max + 100;
        _x_max += dx;
        _x_min += dx;
        chart()->axisX()->setRange(_x_min, _x_max);

        // Removing out of range data to avoid uncontrolled CPU usage
        while(_series->at(0).x() < _x_min)
            _series->remove(0);

        // qDebug() << "Series size : " << QString::number(_series->points().size());
    }


    // Rescale y axis if values goes too high
    if(y >= _y_max - 10) {
        int dy = y - _y_max + 10;
        _y_max += dy;
        chart()->axisY()->setRange(_y_min, _y_max);
    } else if(y <= _y_min - 10) {
        int dy = y + _y_min - 10;
        _y_min += dy;
        chart()->axisY()->setRange(_y_min, _y_max);
    }

    // draw a horizontal line to show the average value over the lasts 10 seconds
    /*
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
    */
}

void RealTimePlot::clear_data() {
    _series->clear();
}
