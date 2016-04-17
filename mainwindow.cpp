#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(setScale(int)));
    connect(ui->rotationSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(setRotationSpeed(int)));
    connect(ui->rotationSpeedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixedSpeed(bool)));
    connect(ui->sceneLightingCheckBox, SIGNAL(toggled(bool)), this, SLOT(setLighting(bool)));

    connect(ui->openFileButton, SIGNAL(clicked(bool)), this, SLOT(openFileDialog(bool)));

    _camera = new Camera(
                QVector3D(0, 0, 1), // eye
                QVector3D(0, 0, 0), // center
                QVector3D(0, 1, 0),  // up

                45, // viewing angle
                1, // zNear plane
                10000 // zFar plane
    );

    _camera->setRotationSpeed(0); // in degrees
    _camera->setRotationAxis(_camera->up());
    ui->renderWidget->setCamera(_camera);

    ui->scaleSlider->setValue(70); // set default scale
    ui->rotationSpeedSlider->setValue(10); // set default rotation speed
    ui->rotationSpeedCheckBox->setCheckState(Qt::Unchecked);
    ui->freeCameraCheckBox->setCheckState(Qt::Unchecked);
    ui->sceneLightingCheckBox->setCheckState(Qt::Unchecked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::timerEvent(QTimerEvent *) {
    if (_camera->rotationSpeed() < 1) {
        return;
    }

    if (!_camera->rotationSpeedFixed()) {
        _camera->setRotationSpeed(_camera->rotationSpeed() - 5.0 / fps); // -5 degrees per second
        ui->rotationSpeedSlider->setValue(_camera->rotationSpeed());
    }

    QQuaternion new_rotation = QQuaternion::fromAxisAndAngle(_camera->rotationAxis(), _camera->rotationSpeed() / fps);

    _camera->setRotation(new_rotation);
    _camera->rotate(_camera->rotation());

    ui->renderWidget->update();
}


void MainWindow::setScale(int sliderPosition) {
    if (sliderPosition == 0)
        sliderPosition = 1;

    _camera->setEye(_camera->center() - _camera->dir() * (100.0f / sliderPosition) * (100.f / sliderPosition));

    ui->renderWidget->update();
}


void MainWindow::setRotationSpeed(int sliderPosition) {
    if (fabs(_camera->rotationSpeed() - sliderPosition) < 1)
        return;

    _camera->setRotationSpeed(sliderPosition);
    ui->renderWidget->update();
}

void MainWindow::setFixedSpeed(bool fixed) {
    _camera->setRotationSpeedFixed(fixed);
    ui->renderWidget->update();
}

void MainWindow::setLighting(bool lighting) {
    ui->renderWidget->lighting = lighting;
    ui->renderWidget->update();
}

void MainWindow::keyPressEvent(QKeyEvent *) {

}

void MainWindow::keyReleaseEvent(QKeyEvent *) {

}

void MainWindow::parseRenderWidgetMouseMoveEvent(QVector2D * v) {
//    _camera->turnRight(v->x());
//    _camera->turnUp(v->y());
    *v *= _camera->fovy() * 10;

    QVector3D axis = -(_camera->right() * v->y() + _camera->up() * v->x());
    //axis += _camera->rotationAxis();
    float speed = axis.length();

    _camera->setRotationAxis(axis);

    if ( !_camera->rotationSpeedFixed() )
        _camera->setRotationSpeed(speed); //_rotationSpeed = speed;


}


void MainWindow::openFileDialog(bool) {
//    _timer.stop();

    QString path = QFileDialog::getOpenFileName(this, "Open model", "", "Obj files (*.obj)");

    if (path == "")
        return;

    ui->renderWidget->releaseScene();

    Object3D *scene = new Object3D(path);
    _camera->setCenter(scene->boundingBox.center());
    _camera->setEye(_camera->center() - QVector3D(0, 0, 10));
    _camera->setUp(QVector3D(0, 1, 0));

    ui->renderWidget->setScene(scene);
//    _timer.start();
}
