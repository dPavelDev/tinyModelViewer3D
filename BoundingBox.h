#pragma once
#include <QVector3D>


class BoundingBox
{
public:
    explicit BoundingBox(QVector3D _min = QVector3D(INFINITY, INFINITY, INFINITY), QVector3D _max = QVector3D(-INFINITY, -INFINITY, -INFINITY));
	BoundingBox(const BoundingBox& a, const BoundingBox& b);
    BoundingBox(const BoundingBox& a, const QVector3D& point);

    QVector3D minPoint() const;
    QVector3D maxPoint() const;

	QVector3D center() const;
	QVector3D dim() const;

	float volume() const;
	float max_dim() const;

    bool isValid() const;
private:
    QVector3D _minPoint, _maxPoint;
};

