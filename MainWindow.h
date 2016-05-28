#pragma once

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <QMainWindow>
#include <QBasicTimer>
#include <qelapsedtimer.h>

#include "Scene.h"

#include <QKeyEvent>


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void timerEvent(QTimerEvent*);

private slots:
    void setScale(int sliderPosition) const;
    void setRotationSpeed(int sliderPosition) const;
    void setFixedSpeed(bool isFixedSpeed) const;
    void setLighting(bool isLighting) const;
    void setCameraMode(bool isFreeCamera) const;
    void setAlignedCameraView(bool isAlignedMode);
    void setPowerSaveMode(bool isPowerSaveMode);

    void openFileDialog(bool);

    void parseRenderWidgetMousePressEvent(QMouseEvent* e);
    void parseRenderWidgetMouseMoveEvent(QMouseEvent* v);
    void parseRenderWidgetMouseReleaseEvent(QMouseEvent*);

    void setMouseSensivity(int value);
    void setKeyboardSensivity(int value);

    void on_aboutProgramAction_triggered();
    void on_changeBackgroundColorButton_clicked();
    void initializeWindow();
    void setDefaultSettings();

private:
    Ui::MainWindow* ui = nullptr;
    QBasicTimer _timer;

    QSet<int> _pressedKeys;
    bool _moveCamera();

    QVector2D _currentMousePos, _currentMousePosDelta;
    bool _isActiveRotate = false;
    float _mouseSensivity = 0;
    float _keyboardSensevity = 0;
    bool _isAlignedMode = false;

    float _minScale = 0.125, _maxScale = 4;

    void _cleanup();
    void _updateTitle();
};
