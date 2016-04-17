#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include <QMessageBox>
#include <QFileDialog>

#include <QOpenGLWidget>

#include "object3d.h"

#include "camera.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QBasicTimer _timer;
    const int fps = 60;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void timerEvent(QTimerEvent *);
private slots:
    void setScale(int sliderPosition);
    void setRotationSpeed(int sliderPosition);
    void setFixedSpeed(bool fixed);
    void setLighting(bool lighting);

    void openFileDialog(bool);

    //void parseRenderWidgetMousePressEvent(QEvent * e);
    void parseRenderWidgetMouseMoveEvent(QVector2D *v);
    //void parseRenderWidgetMouseReleaseEvent(QEvent * e);

private:
    Ui::MainWindow *ui;

    Camera * _camera;
};

#endif // MAINWINDOW_H
