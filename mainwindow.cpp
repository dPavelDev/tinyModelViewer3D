#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <ctime>
#include <QColorDialog>
#include <LoadObjectStatusDialog.h>


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->showMessage("Инициализация...");
    connect(ui->renderWidget, SIGNAL(initialized()), this, SLOT(initializeWindow()));
}

MainWindow::~MainWindow()
{
    _cleanup();
}

void MainWindow::timerEvent(QTimerEvent*)
{
    if(_moveCamera())
        ui->renderWidget->update();

    _updateTitle();
}


void MainWindow::setScale(int sliderPosition) const
{
    if (ui->renderWidget->getScene() == nullptr)
        return;

    float maxDim = ui->renderWidget->getScene()->boundingBox().max_dim();

    float dst = maxDim / 2 / tan(_camera->fovy() / 2);
    float minScale = 0.125, maxScale = 8;

    dst = dst * (maxScale + 1) - (dst * (maxScale + 1) - dst * minScale) * sliderPosition / 100.0f;

    QVector3D newEyePos = _camera->center() - _camera->dir() * dst;

    _camera->setEye(newEyePos);

	ui->renderWidget->update();
}


void MainWindow::setRotationSpeed(int sliderPosition) const
{
	if (fabs(_camera->rotationSpeed() - sliderPosition) < 1)
		return;

	_camera->setRotationSpeed(sliderPosition);
	ui->renderWidget->update();
}

void MainWindow::setFixedSpeed(bool isFixedSpeed) const
{
    _camera->setRotationSpeedFixed(isFixedSpeed);

	ui->renderWidget->update();
}

void MainWindow::setLighting(bool lighting) const
{
	ui->renderWidget->lighting = lighting;
	ui->renderWidget->update();
}

void MainWindow::setCameraMode(bool isFreeCamera) const
{
    _camera->setFixedCenter(!isFreeCamera);

    if (_camera->isFixedCenter())
        _camera->setCenter(ui->renderWidget->getScene()->boundingBox().center());

    ui->scaleSlider->setDisabled(isFreeCamera);
    ui->renderWidget->update();
}

void MainWindow::setAlignedCameraView(bool isAlignedMode)
{
    _isAlignedMode = isAlignedMode;
    if (isAlignedMode)
        _camera->alignUp();

    ui->renderWidget->update();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    _pressedKeys.insert(event->key());

    bool isMove = false;
    isMove |= _pressedKeys.contains(Qt::Key_W);
    isMove |= _pressedKeys.contains(Qt::Key_A);
    isMove |= _pressedKeys.contains(Qt::Key_S);
    isMove |= _pressedKeys.contains(Qt::Key_D);

    if (isMove) {
        ui->freeCameraCheckBox->setChecked(true);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    _pressedKeys.remove(event->key()); // return false if not found
}

void MainWindow::parseRenderWidgetMouseMoveEvent(QMouseEvent *v)
{
    if (!_isActiveRotate)
        return;

    _currentMousePosDelta = QVector2D(v->pos()) - _currentMousePos;
    _currentMousePos = QVector2D(v->pos());

    QVector3D axis = -(_camera->right() * _currentMousePosDelta.y() + _camera->up() * _currentMousePosDelta.x());
    float speed = axis.length() * _mouseSensivity;

	_camera->rotate(QQuaternion::fromAxisAndAngle(axis, speed));

    if (_isAlignedMode)
        _camera->alignUp();

    ui->renderWidget->update();
}


void MainWindow::parseRenderWidgetMousePressEvent(QMouseEvent *e)
{
	_currentMousePos = QVector2D(e->pos());
	_currentMousePosDelta = QVector2D(0, 0);

    _isActiveRotate = 1;
}

void MainWindow::parseRenderWidgetMouseReleaseEvent(QMouseEvent *)
{
    _isActiveRotate = 0;
}

void MainWindow::setMouseSensivity(int value)
{
    _mouseSensivity = value / 50.0f;
}

bool MainWindow::_moveCamera()
{
    auto step = ui->renderWidget->getScene()->boundingBox().max_dim() / 100.f;

    bool isMoved = false;

    if (_pressedKeys.contains(Qt::Key_W))
	{
		_camera->moveForward(step);
        isMoved = true;
	}
    if (_pressedKeys.contains(Qt::Key_S))
	{
		_camera->moveBackward(step);
        isMoved = true;
	}
    if (_pressedKeys.contains(Qt::Key_A))
	{
		_camera->moveLeft(step);
        isMoved = true;
	}
    if (_pressedKeys.contains(Qt::Key_D))
	{
		_camera->moveRight(step);
        isMoved = true;
	}

    return isMoved;
}

void MainWindow::_cleanup()
{
    delete ui;
    delete _camera;
}

void MainWindow::_updateTitle()
{
    static int t = time(nullptr);
    int currentTime = time(nullptr);
    static int lastFrameCount = 0;

    if (t == currentTime)
        return;

    int newFrameCount = ui->renderWidget->getFrameCount();
    setWindowTitle(QString::asprintf("FPS: %d - tinyModelViewer3D", newFrameCount - lastFrameCount));
    t = currentTime;
    lastFrameCount = newFrameCount;
}

void MainWindow::openFileDialog(bool)
{
    ui->renderWidget->pauseTimer();

    auto path = QFileDialog::getOpenFileName(this, "Open model", "", "Obj files (*.obj);;All files (*.*)");

    if (path == "")
        return;

    ui->renderWidget->releaseScene();

    auto scene = new Scene(path);
    _camera->setCenter(scene->boundingBox().center());
    _camera->setEye(scene->boundingBox().maxPoint());
    _camera->setUp(QVector3D(0, 1, 0));

    _camera->setzFar(scene->boundingBox().max_dim() * 100);
    _camera->setzNear(_camera->zFar() / (1 << 16));

    ui->renderWidget->updateView();
    ui->renderWidget->setScene(scene);

    ui->scaleSlider->setValue(0);
    ui->scaleSlider->setValue(70);
	ui->renderWidget->update();
    ui->renderWidget->resumeTimer();
}


void MainWindow::on_aboutProgramAction_triggered()
{
    if (aboutwindow != nullptr)
        delete aboutwindow;

    aboutwindow = new AboutWindow(this);
    aboutwindow->show();
}

void MainWindow::on_changeBackgroundColorButton_clicked()
{
    ui->renderWidget->pauseTimer();

    auto color = QColorDialog::getColor(this->palette().background().color(), this, tr("Выберите цвет фона"));
    if (color.isValid())
        ui->renderWidget->setBackgroundColor(color);

    ui->renderWidget->update();

    ui->renderWidget->resumeTimer();
}

void MainWindow::initializeWindow()
{
    connect(ui->scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(setScale(int)));
    connect(ui->sceneLightingCheckBox, SIGNAL(toggled(bool)), this, SLOT(setLighting(bool)));
    connect(ui->freeCameraCheckBox, SIGNAL(toggled(bool)), this, SLOT(setCameraMode(bool)));

    connect(ui->openFileButton, SIGNAL(clicked(bool)), this, SLOT(openFileDialog(bool)));
    connect(ui->alignCameraCheckBox, SIGNAL(toggled(bool)), this, SLOT(setAlignedCameraView(bool)));

    connect(ui->renderWidget, SIGNAL(mousePressSignal(QMouseEvent*)), this, SLOT(parseRenderWidgetMousePressEvent(QMouseEvent*)));
    connect(ui->renderWidget, SIGNAL(mouseMoveSignal(QMouseEvent*)), this, SLOT(parseRenderWidgetMouseMoveEvent(QMouseEvent*)));
    connect(ui->renderWidget, SIGNAL(mouseReleaseSignal(QMouseEvent*)), this, SLOT(parseRenderWidgetMouseReleaseEvent(QMouseEvent*)));

    connect(ui->mouseSensivitySlider, SIGNAL(valueChanged(int)), this, SLOT(setMouseSensivity(int)));


    _camera = new Camera(
        QVector3D(0, 0, 1), // eye
        QVector3D(0, 0, 0), // center
        QVector3D(0, 1, 0), // up

        45.f, // viewing angle
        0.01f, // zNear plane
        100.f // zFar plane
    );

    _camera->setRotationSpeed(0); // in degrees
    _camera->setRotationAxis(_camera->up());
    ui->renderWidget->setCamera(_camera);

    ui->alignCameraCheckBox->setCheckState(Qt::Checked);

    ui->scaleSlider->setValue(70); // set default scale
    ui->mouseSensivitySlider->setValue(50);
    ui->freeCameraCheckBox->setCheckState(Qt::Unchecked);
    ui->sceneLightingCheckBox->setCheckState(Qt::Unchecked);


    //ui->renderWidget->startTimer(1);
    _timer.start(15, Qt::PreciseTimer, this);

    ui->statusBar->showMessage("Поворот модели левой кнопкой мыши + перемещение");
}
