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


QVector3D Camera::eye() const {
    return _eye;
}
void Camera::setEye(const QVector3D & eye) {
    _eye = eye;
}

QVector3D Camera::center() const {
    return _center;
}
void Camera::setCenter(const QVector3D & center) {
    _center = center;
}

QVector3D Camera::up() const {
    return _up;
}
void Camera::setUp(const QVector3D & up) {
    _up = up;
}

QVector3D Camera::dir() const {
    return (_center - _eye).normalized();
}
QVector3D Camera::right() const {
    return QVector3D::crossProduct(_center - _eye, _up).normalized();
}

float Camera::fovy() {
    return _fovy;
}
void Camera::setFovy(float fovy) {
    _fovy = fovy;
}

float Camera::zNear() const {
    return _zNear;
}
void Camera::setzNear(float zNear) {
    _zNear = zNear;
}

float Camera::zFar() const {
    return _zFar;
}
void Camera::setzFar(float zFar) {
    _zFar = zFar;
}

float Camera::rotationSpeed() const {
    return _rotationSpeed;
}
void Camera::setRotationSpeed(const float &rotationSpeed) {
    _rotationSpeed = rotationSpeed;
}

QVector3D Camera::rotationAxis() const {
    return _rotationAxis;
}
void Camera::setRotationAxis(const QVector3D &rotationAxis) {
    _rotationAxis = rotationAxis;
}

QQuaternion Camera::rotation() const {
    return _rotation;
}
void Camera::setRotation(const QQuaternion &q) {
    _rotation = q;
}

bool Camera::rotationSpeedFixed() const {
    return _rotationSpeedFixed;
}
void Camera::setRotationSpeedFixed(bool isFixed) {
    _rotationSpeedFixed = isFixed;
}

void Camera::rotate(const QQuaternion & quaternion) {
    QVector3D dir = _center - _eye;
    dir = quaternion.rotatedVector(dir);

    _eye = _center - dir;

    _up = quaternion.rotatedVector(_up);
}

void Camera::turnClockwise(const float &angle) {
    this->turnCounterClockwise(-angle);
}
void Camera::turnCounterClockwise(const float &angle) {
    QQuaternion q;
    q = QQuaternion::fromAxisAndAngle(this->dir(), angle);
    this->rotate(q);
}

void Camera::turnUp(const float &angle) {
    QQuaternion q(angle, this->right());
    q = QQuaternion::fromAxisAndAngle(this->right(), angle);
    this->rotate(q);
}
void Camera::turnDown(const float &angle) {
    this->turnUp(-angle);
}

void Camera::turnRight(const float &angle) {
    QQuaternion q;
    q = QQuaternion::fromAxisAndAngle(this->up(), angle);
    this->rotate(q);
}
void Camera::turnLeft(const float &angle) {
    this->turnRight(-angle);
}

void Camera::setLookAt() {
    gluLookAt(_eye.x(), _eye.y(), _eye.z(),
              _center.x(), _center.y(), _center.z(),
              _up.x(), _up.y(), _up.z());
}
