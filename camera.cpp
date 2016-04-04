#include "camera.h"

Camera::Camera(QVector3D eye, QVector3D center, QVector3D up, float fovy, float zNear, float zFar)
{
    _eye = eye;
    _center = center;
    _up = up;

    _fovy = fovy;
    _zNear = zNear;
    _zFar = zFar;
}

Camera::Camera () {
}


QVector3D Camera::eye() {
    return _eye;
}

QVector3D Camera::center() {
    return _center;
}

QVector3D Camera::up() {
    return _up;
}

QVector3D Camera::dir() {
    return (_center - _eye).normalized();
}

float Camera::fovy() {
    return _fovy;
}

float Camera::zNear() {
    return _zNear;
}

float Camera::zFar() {
    return _zFar;
}

void Camera::setEye(const QVector3D & eye) {
    _eye = eye;
}

void Camera::setCenter(const QVector3D & center) {
    _center = center;
}

void Camera::setUp(const QVector3D & up) {
    _up = up;
}

void Camera::setFovy(float fovy) {
    _fovy = fovy;
}

void Camera::setzNear(float zNear) {
    _zNear = zNear;
}

void Camera::setzFar(float zFar) {
    _zFar = zFar;
}

QVector3D Camera::right() {
    return QVector3D::crossProduct(_center - _eye, _up).normalized();
}

void Camera::rotate(const QQuaternion & quaternion) {
    QVector3D dir = quaternion.rotatedVector(_center - _eye);
    _eye = _center - dir;

    _up = quaternion.rotatedVector(_up);
}

void Camera::setGluLookAt() {
    gluLookAt(_eye.x(), _eye.y(), _eye.z(),
              _center.x(), _center.y(), _center.z(),
              _up.x(), _up.y(), _up.z());
}
