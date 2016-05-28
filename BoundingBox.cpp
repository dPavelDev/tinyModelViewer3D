#include "BoundingBox.h"

using std::min;
using std::max;

BoundingBox::BoundingBox()
{
    _minPoint = QVector3D(INFINITY, INFINITY, INFINITY);
    _maxPoint = QVector3D(-INFINITY, -INFINITY, -INFINITY);
}

BoundingBox::BoundingBox(const QVector3D &minPoint, const QVector3D &maxPoint)
{
    _minPoint = minPoint;
    _maxPoint = maxPoint;
}

BoundingBox::BoundingBox(const BoundingBox& box1, const BoundingBox& box2)
{
    _minPoint = QVector3D(
                min(box1._minPoint.x(), box2._minPoint.x()),
                min(box1._minPoint.y(), box2._minPoint.y()),
                min(box1._minPoint.z(), box2._minPoint.z())
                );

    _maxPoint = QVector3D(
                max(box1._maxPoint.x(), box2._maxPoint.x()),
                max(box1._maxPoint.y(), box2._maxPoint.y()),
                max(box1._maxPoint.z(), box2._maxPoint.z())
                );
}

BoundingBox::BoundingBox(const BoundingBox& box, const QVector3D& point)
{
    _minPoint = QVector3D(
                min(box._minPoint.x(), point.x()),
                min(box._minPoint.y(), point.y()),
                min(box._minPoint.z(), point.z())
                );


    _maxPoint = QVector3D(
                max(box._maxPoint.x(), point.x()),
                max(box._maxPoint.y(), point.y()),
                max(box._maxPoint.z(), point.z())
                );
}

QVector3D BoundingBox::minPoint() const
{
    return _minPoint;
}

QVector3D BoundingBox::maxPoint() const
{
    return _maxPoint;
}

QVector3D BoundingBox::center() const
{
    return (_minPoint + _maxPoint) / 2;
}

QVector3D BoundingBox::dimensions() const
{
    return _maxPoint - _minPoint;
}

float BoundingBox::volume() const
{
    auto d = this->dimensions();
    return d.x() * d.y() * d.z();
}

float BoundingBox::maxDimensionSize() const
{
    auto d = this->dimensions();

    return max(d.x(), max(d.y(), d.z()));
}

bool BoundingBox::isValid() const
{
    return _minPoint.x() <= _maxPoint.x() && _minPoint.y() <= _maxPoint.y() && _minPoint.z() <= _maxPoint.z();
}

