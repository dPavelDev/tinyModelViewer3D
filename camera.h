#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QQuaternion>

#include <GL/glu.h> // for Camera::setGluLookAt (const quaternion) function

class Camera
{
public:
    Camera ();
    Camera(QVector3D eye, QVector3D center, QVector3D up, float fovy, float zNear, float zFar);

    QVector3D eye();
    QVector3D center();
    QVector3D up();
    QVector3D dir();
    QVector3D right();

    float fovy();
    float zNear();
    float zFar();

    void setEye(const QVector3D & eye);
    void setCenter(const QVector3D & center);
    void setUp(const QVector3D & up);

    void setFovy(float fovy);
    void setzNear(float zNear);
    void setzFar(float zFar);

    void setGluLookAt();

    void rotate(const QQuaternion & quaternion);
private:
    QVector3D _eye, _center, _up;
    float _fovy = 45, _zNear = 0.001, _zFar = 100;
};

#endif // CAMERA_H
