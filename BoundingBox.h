#pragma once

#include <QVector3D>

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const QVector3D &minPoint, const QVector3D &maxPoint);
    BoundingBox(const BoundingBox& box1, const BoundingBox& box2);
    BoundingBox(const BoundingBox& box, const QVector3D& point);

    QVector3D minPoint() const;
    QVector3D maxPoint() const;

    QVector3D center() const;
    QVector3D dimensions() const; // return length by x, y and z

    float volume() const;
    float maxDimensionSize() const;

    bool isValid() const;
private:
    QVector3D _minPoint, _maxPoint;
};

