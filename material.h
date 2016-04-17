#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLTexture>
#include <QImage>
#include <QColor>

class Material {
public:
    QColor ambientColor, diffuseColor, specularColor, emissionColor;
    GLuint ambientTexture_id, diffuseTexture_id, specularTexture_id, emissionTexture_id, bumpTexture_id;

    float opticalDensity, // ni
          specularExponent, // ns
          transperancy;

    int illuminationMode;

    Material ();
    ~Material ();

    void initTextures();
};

#endif // MATERIAL_H
