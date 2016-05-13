#ifdef _MSC_VER
#include <windows.h>
#endif

#include <GL/glu.h> // for Camera::setGluLookAt (const quaternion) function
#include "camera.h"

Camera::Camera(QVector3D eye, QVector3D center, QVector3D up, float fovy)
{
	_eye = eye;
	_center = center;
	_up = up;

	_fovy = fovy;
}

Camera::Camera()
{
}


QVector3D Camera::eye() const
{
	return _eye;
}

void Camera::setEye(const QVector3D& eye)
{
	_eye = eye;
}

QVector3D Camera::center() const
{
	return _center;
}

void Camera::setCenter(const QVector3D& center)
{
	_center = center;
}

QVector3D Camera::up() const
{
	return _up;
}

void Camera::setUp(const QVector3D& up)
{
	_up = up;
}

QVector3D Camera::dir() const
{
	return (_center - _eye).normalized();
}

QVector3D Camera::right() const
{
	return QVector3D::crossProduct(_center - _eye, _up).normalized();
}

float Camera::fovy() const
{
	return _fovy;
}

void Camera::setFovy(float fovy)
{
	_fovy = fovy;
}

float Camera::zNear() const
{
	return _zNear;
}

void Camera::setzNear(float zNear)
{
	_zNear = zNear;
}

float Camera::zFar() const
{
	return _zFar;
}

void Camera::setzFar(float zFar)
{
	_zFar = zFar;
}

float Camera::rotationSpeed() const
{
	return _rotationSpeed;
}

void Camera::setRotationSpeed(const float& rotationSpeed)
{
	_rotationSpeed = rotationSpeed;
}

QVector3D Camera::rotationAxis() const
{
	return _rotationAxis;
}

void Camera::setRotationAxis(const QVector3D& rotationAxis)
{
	_rotationAxis = rotationAxis;
}

QQuaternion Camera::rotation() const
{
	return _rotation;
}

void Camera::setRotation(const QQuaternion& q)
{
	_rotation = q;
}

bool Camera::isFixedCenter() const
{
	return _isFixedCenter;
}

void Camera::setFixedCenter(bool fixed)
{
	_isFixedCenter = fixed;
}

bool Camera::rotationSpeedFixed() const
{
	return _rotationSpeedFixed;
}

void Camera::setRotationSpeedFixed(bool isFixed)
{
	_rotationSpeedFixed = isFixed;
}

void Camera::rotate(const QQuaternion& quaternion)
{
	auto dir = _center - _eye;
	dir = quaternion.rotatedVector(dir);

	if (_isFixedCenter)
		_eye = _center - dir;
	else
		_center = _eye + dir;

	_up = quaternion.rotatedVector(_up);
}

void Camera::turnClockwise(const float& angle)
{
	this->turnCounterClockwise(-angle);
}

void Camera::turnCounterClockwise(const float& angle)
{
	QQuaternion q;
	q = QQuaternion::fromAxisAndAngle(this->dir(), angle);
	this->rotate(q);
}

void Camera::moveOnVector(const QVector3D& v)
{
	_eye += v;
	_center += v;
}

void Camera::moveForward(const float& dist)
{
	moveOnVector(dir() * dist);
}

void Camera::moveBackward(const float& dist)
{
	moveOnVector(-dir() * dist);
}

void Camera::moveLeft(const float& dist)
{
	moveOnVector(-right() * dist);
}

void Camera::moveRight(const float& dist)
{
	moveOnVector(right() * dist);
}

void Camera::moveDown(const float& dist)
{
	moveOnVector(-up() * dist);
}

void Camera::moveUp(const float& dist)
{
	moveOnVector(up() * dist);
}

void Camera::alignUp()
{
	this->setUp(QVector3D(0, 1, 0));
	auto left = QVector3D::crossProduct(this->up(), this->dir());
	this->setUp(QVector3D::crossProduct(this->dir(), left));
}

void Camera::turnUp(const float& angle)
{
	QQuaternion q;
	q = QQuaternion::fromAxisAndAngle(this->right(), angle);
	this->rotate(q);
}

void Camera::turnDown(const float& angle)
{
	this->turnUp(-angle);
}

void Camera::turnRight(const float& angle)
{
	QQuaternion q;
	q = QQuaternion::fromAxisAndAngle(this->up(), angle);
	this->rotate(q);
}

void Camera::turnLeft(const float& angle)
{
	this->turnRight(-angle);
}

void Camera::setLookAt() const
{
	gluLookAt(_eye.x(), _eye.y(), _eye.z(),
	          _center.x(), _center.y(), _center.z(),
	          _up.x(), _up.y(), _up.z());
}

