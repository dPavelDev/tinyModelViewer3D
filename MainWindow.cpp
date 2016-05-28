#include "LoadObjectStatusDialog.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <ctime>
#include <QColorDialog>
#include <QMessageBox>
#include <QElapsedTimer>

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
    if (_moveCamera())
    {
        ui->renderWidget->updateLater();
    }

    _updateTitle();
}


void MainWindow::setScale(int sliderPosition) const
{
    auto scene = ui->renderWidget->scene();
    auto camera = ui->renderWidget->camera();

    if (scene == nullptr || camera == nullptr)
        return;

    // calculate minimal distance when object completely visible in perspective camera
    float maxDim = scene->boundingBox().maxDimensionSize();
    float dst = maxDim / 2 / tan(camera->fovy() / 2);

    float realSliderPosition = sliderPosition / 100.0;

    dst = dst * (_maxScale + 1) - (dst * (_maxScale + 1) - dst * _minScale) * realSliderPosition; // linear interpolation

    // calculate new eye pos
    QVector3D newEyePos = camera->center() - camera->dir() * dst;

    camera->setEye(newEyePos);

    ui->renderWidget->updateLater();
}


void MainWindow::setRotationSpeed(int sliderPosition) const
{
    auto camera = ui->renderWidget->camera();

    if (camera == nullptr)
        return;

    if (fabs(camera->rotationSpeed() - sliderPosition) < 1)
        return;

    camera->setRotationSpeed(sliderPosition);
    ui->renderWidget->updateLater();
}

void MainWindow::setFixedSpeed(bool isFixedSpeed) const
{
    auto camera = ui->renderWidget->camera();

    if (camera == nullptr)
        return;


    camera->setRotationSpeedFixed(isFixedSpeed);

    ui->renderWidget->updateLater();
}

void MainWindow::setLighting(bool isLighting) const
{
    ui->renderWidget->setLighting(isLighting);
    ui->renderWidget->updateLater();
}

void MainWindow::setCameraMode(bool isFreeCamera) const
{
    auto camera = ui->renderWidget->camera();
    auto scene = ui->renderWidget->scene();

    if (camera == nullptr || scene == nullptr)
        return;

    camera->setFixedCenter(!isFreeCamera);

    if (camera->isFixedCenter())
        camera->setCenter(scene->boundingBox().center());

    ui->scaleSlider->setDisabled(isFreeCamera);
    ui->renderWidget->updateLater();
}

void MainWindow::setAlignedCameraView(bool isAlignedMode)
{
    auto camera = ui->renderWidget->camera();

    if (camera == nullptr)
        return;

    _isAlignedMode = isAlignedMode;
    if (isAlignedMode)
        camera->alignUp();

    ui->renderWidget->updateLater();
}

void MainWindow::setPowerSaveMode(bool isPowerSaveMode)
{
    ui->renderWidget->setPowerSaveMode(isPowerSaveMode);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (_pressedKeys.contains(event->key()))
        return;

    _pressedKeys.insert(event->key());

    bool isMove = false;
    isMove |= _pressedKeys.contains(Qt::Key_W);
    isMove |= _pressedKeys.contains(Qt::Key_A);
    isMove |= _pressedKeys.contains(Qt::Key_S);
    isMove |= _pressedKeys.contains(Qt::Key_D);

    if (isMove && ui->freeCameraCheckBox->checkState() == Qt::Unchecked)
    {
        ui->freeCameraCheckBox->setChecked(true);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    _pressedKeys.remove(event->key()); // return false if not found
}

void MainWindow::parseRenderWidgetMouseMoveEvent(QMouseEvent* v)
{
    if (!_isActiveRotate)
        return;

    auto camera = ui->renderWidget->camera();

    if (camera == nullptr)
        return;

    _currentMousePosDelta = QVector2D(v->pos()) - _currentMousePos;
    _currentMousePos = QVector2D(v->pos());

    if (_isAlignedMode) {
        QVector3D alignVector = QVector3D(0, 1, 0);
        float angle = QVector3D::dotProduct(alignVector, camera->up());

        if ( angle < 0.1f) {
            if (QVector3D::dotProduct(alignVector, camera->dir()) < 0 && _currentMousePosDelta.y() > 0)
                _currentMousePosDelta.setY(0);
            if (QVector3D::dotProduct(alignVector, camera->dir()) > 0 && _currentMousePosDelta.y() < 0)
                _currentMousePosDelta.setY(0);
        }
    }

    // rotation axis is perpendicular to mouse vector
    QVector3D rotationAxis = -(camera->right() * _currentMousePosDelta.y() + camera->up() * _currentMousePosDelta.x());
    float speed = rotationAxis.length() * _mouseSensivity;



    camera->rotate(QQuaternion::fromAxisAndAngle(rotationAxis, speed));

    if (_isAlignedMode)
        camera->alignUp();

    ui->renderWidget->updateLater();
}

void MainWindow::parseRenderWidgetMousePressEvent(QMouseEvent* e)
{
    _currentMousePos = QVector2D(e->pos());
    _currentMousePosDelta = QVector2D(0, 0);

    _isActiveRotate = 1;
}

void MainWindow::parseRenderWidgetMouseReleaseEvent(QMouseEvent*)
{
    _isActiveRotate = 0;
}

void MainWindow::setMouseSensivity(int value)
{
    if (value == 0)
        value = 1;

    _mouseSensivity = value * value / 5000.0f;
}

void MainWindow::setKeyboardSensivity(int value)
{
    if (value == 0)
        value = 1;

    _keyboardSensevity = value * value / 10000.0f;
}

bool MainWindow::_moveCamera()
{
    auto scene = ui->renderWidget->scene();
    auto camera = ui->renderWidget->camera();

    if (scene == nullptr || camera == nullptr)
        return false;

    auto step = scene->boundingBox().maxDimensionSize() * _keyboardSensevity;

    bool isMoved = false;

    float max_dist = (_maxScale + 1) * scene->boundingBox().maxDimensionSize();

    if (_pressedKeys.contains(Qt::Key_W))
    {
        camera->moveForward(step);

        if ( camera->eye().distanceToPoint(scene->boundingBox().center()) > max_dist )
            camera->moveForward(-step);
        isMoved = true;
    }
    if (_pressedKeys.contains(Qt::Key_S))
    {
        camera->moveBackward(step);

        if ( camera->eye().distanceToPoint(scene->boundingBox().center()) > max_dist )
            camera->moveBackward(-step);
        isMoved = true;
    }
    if (_pressedKeys.contains(Qt::Key_A))
    {
        camera->moveLeft(step);

        if ( camera->eye().distanceToPoint(scene->boundingBox().center()) > max_dist )
            camera->moveLeft(-step);
        isMoved = true;
    }
    if (_pressedKeys.contains(Qt::Key_D))
    {
        camera->moveRight(step);

        if ( camera->eye().distanceToPoint(scene->boundingBox().center()) > max_dist )
            camera->moveRight(-step);
        isMoved = true;
    }

    scene->setLightPos(camera->eye());

    return isMoved;
}

void MainWindow::_cleanup()
{
    delete ui;
}

void MainWindow::_updateTitle()
{
    static int t = time(nullptr);
    int currentTime = time(nullptr);
    static int lastFrameCount = 0;

    if (t == currentTime)
        return;

    int newFrameCount = ui->renderWidget->frameCount();
    setWindowTitle(QString("FPS: %1 - tinyModelViewer3D").arg(newFrameCount - lastFrameCount));
    t = currentTime;
    lastFrameCount = newFrameCount;
}

void MainWindow::openFileDialog(bool)
{
    ui->renderWidget->pauseTimer();

    auto path = QFileDialog::getOpenFileName(this, "Open model", "", "Obj files (*.obj);;All files (*.*)");

    if (path != "")
    {
        Scene* scene = nullptr;

        LoadObjectStatusDialog* dlg = new LoadObjectStatusDialog(this, &scene, path);
        dlg->exec();
        delete dlg;

        bool isLoaded = scene != nullptr;

        if (isLoaded)
        {
            delete ui->renderWidget->scene();

            ui->renderWidget->setScene(scene);

            auto camera = ui->renderWidget->camera();

            camera->setCenter(scene->boundingBox().center());
            camera->setEye(scene->boundingBox().maxPoint());
            camera->setUp(QVector3D(0, 1, 0));

            camera->setzFar(scene->boundingBox().maxDimensionSize() * (_maxScale + 1));
            camera->setzNear(camera->zFar() / (1 << 12));

            scene->setLightPos(camera->eye());

            ui->renderWidget->updateView();


            ui->scaleSlider->setValue(0);
            ui->scaleSlider->setValue(70);
        }
        else
        {
            QMessageBox::warning(this, "Внимание!", QString("Файл не загружен: %1").arg(path));
        }
    }
    ui->renderWidget->resumeTimer();
    ui->renderWidget->updateLater();
}


void MainWindow::on_aboutProgramAction_triggered()
{
    QFile aboutText(":/html/about.html");
    aboutText.open(QFile::ReadOnly);

    QMessageBox::about(this, tr("О программе"), tr(aboutText.readAll())	);
}

void MainWindow::on_changeBackgroundColorButton_clicked()
{
    ui->renderWidget->pauseTimer();

    auto color = QColorDialog::getColor(this->palette().background().color(), this, "Выберите цвет фона");
    if (color.isValid())
        ui->renderWidget->setBackgroundColor(color);

    ui->renderWidget->updateLater();

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
    connect(ui->keyboardSensivitySlider, SIGNAL(valueChanged(int)), this, SLOT(setKeyboardSensivity(int)));

    connect(ui->powerSaveModeCheckBox, SIGNAL(toggled(bool)), this, SLOT(setPowerSaveMode(bool)));

    auto scene = new Scene(); // set default model
    scene->genCube();
    ui->renderWidget->setScene(scene);

    setDefaultSettings();

    ui->renderWidget->startTimer(60);
    _timer.start(10, Qt::CoarseTimer, this);

    ui->statusBar->showMessage("Поворот модели левой кнопкой мыши + перемещение");
}

void MainWindow::setDefaultSettings()
{
    auto camera = new Camera(
                QVector3D(0, 0, 1), // eye
                QVector3D(0, 0, 0), // center
                QVector3D(0, 1, 0), // up

                45.f // viewing angle
                );

    auto scene = ui->renderWidget->scene();

    camera->setzFar(scene->boundingBox().maxDimensionSize() * 10);
    camera->setzNear(camera->zFar() / (1 << 16));

    camera->setRotationSpeed(0); // in degrees
    camera->setRotationAxis(camera->up());

    if (ui->renderWidget->camera() != nullptr)
        delete ui->renderWidget->camera();
    ui->renderWidget->setCamera(camera);

    ui->alignCameraCheckBox->setCheckState(Qt::Checked);

    ui->scaleSlider->setValue(70); // set default scale
    ui->mouseSensivitySlider->setValue(20);
    ui->keyboardSensivitySlider->setValue(20);
    ui->freeCameraCheckBox->setCheckState(Qt::Unchecked);
    ui->sceneLightingCheckBox->setCheckState(Qt::Unchecked);
    ui->sceneLightingCheckBox->setCheckState(Qt::Checked);
}

