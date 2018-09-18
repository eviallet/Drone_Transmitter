#include "visualization.h"

Visualization::Visualization(QWidget *parent) : QWidget(parent) {
    Qt3DExtras::Qt3DWindow *window = new Qt3DExtras::Qt3DWindow();
    window->defaultFrameGraph()->setClearColor(QColor(QRgb(0xF0F0F0)));
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();
    window->setRootEntity(rootEntity);

    // Light and camera
    Qt3DRender::QCamera *cameraEntity = window->camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    cameraEntity->tiltAboutViewCenter(30.0f);

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // Cuboid shape data
    Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();
    cuboid->setYExtent(0.1f);
    cuboid->setXExtent(2.0f);
    cuboid->setZExtent(2.0f);

    // CuboidMesh Transform
    _cuboidTransform = new Qt3DCore::QTransform();
    _cuboidTransform->setScale(4.0f);


    Qt3DExtras::QPhongMaterial *cuboidMaterial = new Qt3DExtras::QPhongMaterial();
    cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));

    //Cuboid
    _cube = new Qt3DCore::QEntity(rootEntity);
    _cube->addComponent(cuboid);
    _cube->addComponent(cuboidMaterial);
    _cube->addComponent(_cuboidTransform);


    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(QWidget::createWindowContainer(window));
    setLayout(layout);
}

void Visualization::visualization_state(bool state) {
    _enabled = state;
    if(_last_is_defined)
        update_angles(_last);
}

void Visualization::update_angles(SensorData s) {
    if(_enabled) {
        _cuboidTransform->setRotationX(s.pitch);
        _cuboidTransform->setRotationY(-s.yaw);
        _cuboidTransform->setRotationZ(-s.roll);
    } else {
        _last = s;
        if(!_last_is_defined)
            _last_is_defined=true;
    }
}
