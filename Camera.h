#pragma once

#include <QQuaternion>

class Camera
{
public:
    Camera();
    Camera(QVector3D eye, QVector3D center, QVector3D up, float fovy);

    QVector3D eye() const;
    void setEye(const QVector3D& eye);

    QVector3D center() const;
    void setCenter(const QVector3D& center);

    QVector3D up() const;
    void setUp(const QVector3D& up);

    QVector3D dir() const;
    QVector3D right() const;

    float fovy() const;
    void setFovy(float fovy);

    float zNear() const;
    void setzNear(float zNear);

    float zFar() const;
    void setzFar(float zFar);

    float rotationSpeed() const;
    void setRotationSpeed(const float& rotationSpeed);

    QVector3D rotationAxis() const;
    void setRotationAxis(const QVector3D& rotationAxis);

    bool rotationSpeedFixed() const;
    void setRotationSpeedFixed(bool isRotationSpeedFixed);

    QQuaternion rotation() const;
    void setRotation(const QQuaternion& q);

    bool isFixedCenter() const;
    void setFixedCenter(bool isFixedCenter = true);

    void setLookAt() const;

    void rotate(const QQuaternion& quaternion);
    void turnLeft(const float& angle);
    void turnRight(const float& angle);
    void turnUp(const float& angle);
    void turnDown(const float& angle);
    void turnClockwise(const float& angle);
    void turnCounterClockwise(const float& angle);

    void moveOnVector(const QVector3D& v);

    void moveForward(const float& dist);
    void moveBackward(const float& dist);
    void moveLeft(const float& dist);
    void moveRight(const float& dist);
    void moveDown(const float& dist);
    void moveUp(const float& dist);

    void alignUp(const QVector3D alignVector = QVector3D(0, 1, 0));
private:
    QVector3D _eye, _center, _up;
    float _fovy = 45.0f, _zNear = 0.01f, _zFar = 100.0f; //default values

    bool _isFixedCenter = true;

    QVector3D _rotationAxis;
    float _rotationSpeed = 0;
    bool _rotationSpeedFixed = false;
    QQuaternion _rotation;
};
