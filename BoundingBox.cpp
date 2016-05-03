#include "BoundingBox.h"

BoundingBox::BoundingBox(QVector3D _min, QVector3D _max)
{
    _minPoint = _min;
    _maxPoint = _max;
}

BoundingBox::BoundingBox(const BoundingBox& a, const BoundingBox& b)
{
	using std::min;
	using std::max;

    _minPoint = QVector3D(
        min(a._minPoint.x(), b._minPoint.x()),
        min(a._minPoint.y(), b._minPoint.y()),
        min(a._minPoint.z(), b._minPoint.z())
	);

    _maxPoint = QVector3D(
        max(a._maxPoint.x(), b._maxPoint.x()),
        max(a._maxPoint.y(), b._maxPoint.y()),
        max(a._maxPoint.z(), b._maxPoint.z())
	);
}

BoundingBox::BoundingBox(const BoundingBox& a, const QVector3D& point)
{
	using std::min;
	using std::max;

    _minPoint = QVector3D(
        min(a._minPoint.x(), point.x()),
        min(a._minPoint.y(), point.y()),
        min(a._minPoint.z(), point.z())
	);


    _maxPoint = QVector3D(
        max(a._maxPoint.x(), point.x()),
        max(a._maxPoint.y(), point.y()),
        max(a._maxPoint.z(), point.z())
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

QVector3D BoundingBox::dim() const
{
    return _maxPoint - _minPoint;
}

float BoundingBox::volume() const
{
	auto d = this->dim();
	return d.x() * d.y() * d.z();
}

float BoundingBox::max_dim() const
{
	using std::max;
	auto d = this->dim();

    return max(d.x(), max(d.y(), d.z()));
}

bool BoundingBox::isValid() const
{
    return _minPoint.x() <= _maxPoint.x() && _minPoint.y() <= _maxPoint.y() && _minPoint.z() <= _maxPoint.z();
}

