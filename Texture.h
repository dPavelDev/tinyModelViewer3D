#pragma once

#include <QImage>
#include <QOpenGLTexture>
#include <GL/gl.h>
#include <QMap>
#include <QOpenGLTexture>
#include <QPair>


class Texture
{
public:
    Texture();

    explicit Texture(const QString &texPath);
    ~Texture();

    bool init();
    bool isInit() const;
    bool bind() const;
    GLuint textureId() const;
    bool isNull() const;
private:
    QString _texPath;
    QImage _texImage;
    QOpenGLTexture * _oglTexture = nullptr;

    bool _isInit = false;
};

