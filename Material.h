#pragma once

#ifdef _MSC_VER
#pragma comment(lib, "OPENGL32.LIB")
#include <windows.h>
#endif
#include <GL/gl.h>

#include <QtGui/QColor>

#include <QImage>
#include "Texture.h"

struct Material
{
    QColor ambientColor;
    QColor diffuseColor;
    QColor specularColor;
    QColor emissionColor;
    QColor transmissionFilter;

    Texture *ambientTexture = nullptr;
    Texture *diffuseTexture = nullptr;
    Texture *specularTexture = nullptr;
    Texture *specularExponentTexture = nullptr;
    Texture *emissionTexture = nullptr;
    Texture *bumpTexture = nullptr;
    Texture *opticalDensityTexture = nullptr;
    Texture *dissolveTexture = nullptr;

    float opticalDensity = 1.0f;
    float specularExponent = 1.0f;
    float transperancy = 0;
    float sharpness = 60.0f;

    int illuminationMode = 0;

    Material();
    ~Material();
};

