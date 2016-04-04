#ifndef OGLRENDER_H
#define OGLRENDER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QSurfaceFormat>

#include <QMouseEvent>
#include <QBasicTimer>
#include <QVector3D>
#include <GL/glu.h>
#include <QQuaternion>

#include "camera.h"

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *);
    ~RenderWidget();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *);

    void timerEvent(QTimerEvent *);

private slots:
    void setScale(int sliderPosition);
    void setRotationSpeed(int sliderPosition);
    void setFixedSpeed(bool fixed);

signals:
    void speedChanged(int);

private:
    QBasicTimer _timer;
    const int fps = 60;

    Camera _camera;

    QVector3D _rotationAxis;
    float _rotationSpeed;
    QQuaternion _rotation;
    bool _rotationFixed = false;

    bool _mousePressed = false;
    QPoint _lastPos = QPoint(0, 0);
};

#endif // OGLRENDER_H
