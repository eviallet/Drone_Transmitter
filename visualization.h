// Qt3D documentation : http://doc.qt.io/qt-5/qt3d-basicshapes-cpp-main-cpp.html

#ifndef VISUALIZATION_H
#define VISUALIZATION_H


#include <Qt3DExtras>
#include <QWidget>
#include <QVBoxLayout>
#include "packet.h"

class Visualization : public QWidget {
    Q_OBJECT
public:
    explicit Visualization(QWidget *parent = nullptr);

signals:

public slots:
    void visualization_state(bool state);
    void update_angles(SensorData s);
private:
    SensorData _last;
    bool _last_is_defined = false;
    bool _enabled = false;
    Qt3DCore::QEntity *_cube;
    Qt3DCore::QTransform *_cuboidTransform;
};

#endif // VISUALIZATION_H
