#ifndef MATERIAL_H
#define MATERIAL_H

#ifdef _MSC_VER
#pragma comment(lib, "OPENGL32.LIB")
#include <windows.h>
#endif
#include <GL/gl.h>

#include <QtGui/QColor>

struct Material
{
	QColor ambientColor;
	QColor diffuseColor;
	QColor specularColor;
	QColor emissionColor;
    QColor transmissionFilter;

    GLuint ambientTexture_id = 0;
    GLuint diffuseTexture_id = 0;
    GLuint specularTexture_id = 0;
    GLuint emissionTexture_id = 0;
    GLuint bumpTexture_id = 0;
    GLuint opticalDensityTexture_id = 0;
    GLuint dissolveTexture_id = 0;

    float opticalDensity = 1.0f;
    float specularExponent = 1.0f;
    float transperancy = 0;
    float sharpness = 60.0f;

    int illuminationMode = 0;

	Material();
	~Material();
};

#endif // MATERIAL_H


