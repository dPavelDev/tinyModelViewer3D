#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLTexture>

#include <QString>

#include <vector>
#include <fstream>
#include <QStringRef>
#include <QVector>
#include <iostream>
#include <QFile>
#include <QTextStream>

#include <QFileInfo>
#include <QDir>
#include <QMap>

#include "material.h"

class Triangles {
public:
    std::vector<QVector3D> verticies, normals;
    std::vector<QVector2D> texverticies;

    Triangles () {
        verticies.clear();
        normals.clear();
        texverticies.clear();
    }

    void shrink_to_fit() {
        verticies.shrink_to_fit();
        normals.shrink_to_fit();
        texverticies.shrink_to_fit();
    }

    ~Triangles () {
    }
};

class Quads {
public:
    std::vector<QVector3D> verticies, normals;
    std::vector<QVector2D> texverticies;

    Quads () {
        verticies.clear();
        normals.clear();
        texverticies.clear();
    }


    void shrink_to_fit() {
        verticies.shrink_to_fit();
        normals.shrink_to_fit();
        texverticies.shrink_to_fit();
    }


    ~Quads () {
    }
};

class BoundingBox {
public:
    QVector3D min_point, max_point;

    BoundingBox (QVector3D _min = QVector3D(INFINITY, INFINITY, INFINITY), QVector3D _max = QVector3D(-INFINITY, -INFINITY, -INFINITY)) {
        min_point = _min;
        max_point = _max;
    }

    BoundingBox (const BoundingBox & a, const BoundingBox & b) {
        using std::min;
        using std::max;

        min_point = QVector3D(
                        min(a.min_point.x(), b.min_point.x()),
                        min(a.min_point.y(), b.min_point.y()),
                        min(a.min_point.z(), b.min_point.z())
                    );


        max_point = QVector3D(
                        max(a.max_point.x(), b.max_point.x()),
                        max(a.max_point.y(), b.max_point.y()),
                        max(a.max_point.z(), b.max_point.z())
                    );
    }

    BoundingBox (const BoundingBox & a, const QVector3D & v) {
        using std::min;
        using std::max;

        min_point = QVector3D(
                        min(a.min_point.x(), v.x()),
                        min(a.min_point.y(), v.y()),
                        min(a.min_point.z(), v.z())
                    );


        max_point = QVector3D(
                        max(a.max_point.x(), v.x()),
                        max(a.max_point.y(), v.y()),
                        max(a.max_point.z(), v.z())
                    );
    }

    QVector3D center () const {
        return (min_point + max_point) / 2;
    }
};


class Object3D
{
public:
    class ShapeGroup {
    public:
        Triangles triangles;
        Quads quads;

        Material material;

        ShapeGroup () {

        }
    };

    Object3D ();
    Object3D (const QString & path);
    ~Object3D ();

    void clear();
    bool genCube();
    bool loadObj(const QString & path);
    bool loadMaterialLibrary(const QString & path);

    GLuint loadTexture(const QString & path);

    QVector<ShapeGroup> shapeGroups;
    QMap<QString, int> materialNames;
    QMap<int, QOpenGLTexture *> textures;
    int lastMaterialName = 0;

    BoundingBox boundingBox;
};

#endif // OBJECT3D_H
