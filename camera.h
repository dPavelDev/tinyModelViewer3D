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

    QVector3D eye() const;
    void setEye(const QVector3D & eye);

    QVector3D center() const;
    void setCenter(const QVector3D & center);

    QVector3D up() const;
    void setUp(const QVector3D & up);

    QVector3D dir() const;
    QVector3D right() const;

    float fovy();    
    void setFovy(float fovy);

    float zNear() const;
    void setzNear(float zNear);

    float zFar() const;
    void setzFar(float zFar);

    float rotationSpeed() const;
    void setRotationSpeed(const float & rotationSpeed);

    QVector3D rotationAxis() const;
    void setRotationAxis(const QVector3D & rotationAxis);

    bool rotationSpeedFixed() const;
    void setRotationSpeedFixed(bool isFixed);

    QQuaternion rotation() const;
    void setRotation (const QQuaternion &q);

    void setLookAt();

    void rotate(const QQuaternion & quaternion);
    void turnLeft(const float &angle);
    void turnRight(const float & angle);
    void turnUp(const float &angle);
    void turnDown(const float &angle);
    void turnClockwise(const float & angle);
    void turnCounterClockwise(const float & angle);

    void moveForward(const float & dist);
    void moveBackward(const float & dist);
    void moveLeft(const float & dist);
    void moveRight(const float & dist);
    void moveUp(const float & dist);
    void moveDown(const float & dist);

private:
    QVector3D _eye, _center, _up;
    float _fovy = 45, _zNear = 0.001, _zFar = 100;

    bool _fixedCenter = true;

    QVector3D _rotationAxis;
    float _rotationSpeed = 0;
    bool _rotationSpeedFixed = false;
    QQuaternion _rotation;
};

#endif // CAMERA_H
