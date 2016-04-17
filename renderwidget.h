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
#include <QImage>
#include <QGLContext>
#include <QOpenGLTexture>

#include "object3d.h"

#include "camera.h"
#include "mainwindow.h"

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *);
    ~RenderWidget();

    bool lighting = false;
    QVector3D lightPosition ();
    void setLightPosition (const QVector3D & lightPosition);

    void setCamera(Camera * camera);
    void releaseCamera();

    void setScene(Object3D * scene = nullptr);
    void releaseScene();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void mouseDownMoveSignal(QVector2D *);

private:
    Camera * _camera = nullptr;

    bool _mousePressed = false;
    QPoint _lastPos = QPoint(0, 0);

    Object3D * _scene = 0;
    QVector3D _lightPosition;
};

#endif // OGLRENDER_H
