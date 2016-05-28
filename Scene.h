#pragma once

#include <QVector>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QSet>

#include <QTextStream>

#include "BoundingBox.h"
#include "ShapeGroup.h"

#include "SceneStat.h"

#include "StringUtilities.h"
#include "ShapeGroup.h"
#include "Texture.h"

class Scene
{
public:
    std::vector<ShapeGroup> shapeGroups;

    Scene();
    ~Scene();

    bool genCube();

    void initMaterials();
    void initGeometry();

    QVector3D lightPos() const;
    void setLightPos(const QVector3D& lightPos);

    BoundingBox boundingBox() const;
    void setBoundingBox(const BoundingBox &boundingBox);

    int addShapeGroup();
    Texture * addTexture(const QString &path);
    static bool initTexture(Texture * texture);

    int texturesCount() const;
private:
    QVector3D _lightPos;
    BoundingBox _boundingBox;

    QMap<QString, Texture> _textureCache;
};

