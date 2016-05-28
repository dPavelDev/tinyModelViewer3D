#include <QFile>
#include <QFileInfo>

#include "Scene.h"
#include <QtCore/QDir>
#include "ShapeGroup.h"

Scene::Scene()
{
}

void Scene::initMaterials()
{
    for (ShapeGroup& shapeGroup : shapeGroups)
    {
        Material* material = &shapeGroup.material;

        initTexture(material->ambientTexture);
        initTexture(material->diffuseTexture);
    }
}

void Scene::initGeometry()
{
    for (ShapeGroup & shapeGroup : shapeGroups) {
        shapeGroup.triangles.initBuffers();
    }
}


QVector3D Scene::lightPos() const
{
    return _lightPos;
}

void Scene::setLightPos(const QVector3D& lightPos)
{
    _lightPos = lightPos;
}

BoundingBox Scene::boundingBox() const
{
    return _boundingBox;
}

void Scene::setBoundingBox(const BoundingBox &boundingBox)
{
    _boundingBox = boundingBox;
}

int Scene::addShapeGroup()
{
    shapeGroups.push_back(ShapeGroup());
    return shapeGroups.size() - 1;
}

Texture *Scene::addTexture(const QString &path)
{
    if (!_textureCache.contains(path)) {
        _textureCache.insert(path, Texture(path));
    }

    return &_textureCache[path];
}

bool Scene::initTexture(Texture *texture)
{
    if (texture == nullptr)
        return false;

    return texture->init();
}

int Scene::texturesCount() const
{
    return _textureCache.size();
}


bool Scene::genCube()
{
    addShapeGroup();
    auto& currentShapeGroup = shapeGroups[0];

    QVector<QVector3D> v = {
        {-0.5, -0.5, -0.5},
        {-0.5, -0.5, 0.5},
        {-0.5, 0.5, -0.5},
        {-0.5, 0.5, 0.5},
        {0.5, -0.5, -0.5},
        {0.5, -0.5, 0.5},
        {0.5, 0.5, -0.5},
        {0.5, 0.5, 0.5},
    };

    QVector<QVector2D> vt = {
        {1, 0},
        {0, 0.0},
        {0, 1},
        {1, 1}
    };

    QVector<QVector<int>> f = {
        {3, 1, 5, 7},
        {7, 5, 4, 6},
        {6, 4, 0, 2},
        {2, 0, 1, 3},
        {0, 1, 5, 4},
        {2, 3, 7, 6}
    };


    currentShapeGroup.material.diffuseTexture = addTexture(":/tex/texture.jpg");
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            _boundingBox = BoundingBox(_boundingBox, QVector3D(v[f[i][j]][0], v[f[i][j]][1], v[f[i][j]][2]));

            currentShapeGroup.quads.verticies.push_back(v[f[i][j]]);
            currentShapeGroup.quads.texverticies.push_back(vt[j]);
        }
    }

    return true;
}

Scene::~Scene()
{
}

