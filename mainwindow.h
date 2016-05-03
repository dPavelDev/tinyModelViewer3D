#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <QMainWindow>
#include <QBasicTimer>

#include "scene.h"
#include "camera.h"
#include "AboutWindow.h"

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

	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent* eve) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent*) Q_DECL_OVERRIDE;

private slots:
	void setScale(int sliderPosition) const;
	void setRotationSpeed(int sliderPosition) const;
    void setFixedSpeed(bool isFixedSpeed) const;
	void setLighting(bool lighting) const;
    void setCameraMode(bool isFreeCamera) const;
    void setAlignedCameraView(bool isAlignedMode);

	void openFileDialog(bool);

    void parseRenderWidgetMousePressEvent(QMouseEvent * e);
    void parseRenderWidgetMouseMoveEvent(QMouseEvent* v);
    void parseRenderWidgetMouseReleaseEvent(QMouseEvent *);

    void setMouseSensivity(int value);

    void on_aboutProgramAction_triggered();
    void on_changeBackgroundColorButton_clicked();
    void initializeWindow();
private:
	Ui::MainWindow* ui;
    QBasicTimer _timer;

    AboutWindow * aboutwindow = nullptr;
    Camera* _camera = nullptr;

    QSet<int> _pressedKeys;
    bool _moveCamera();

    QVector2D _currentMousePos, _currentMousePosDelta;
    bool _isActiveRotate = false;
    float _mouseSensivity = 0;
    float _keyboardSensevity = 0;
    bool _isAlignedMode = false;

    void _cleanup();
    void _updateTitle();
};

#endif // MAINWINDOW_H


