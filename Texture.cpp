#include "Texture.h"
#include <QDebug>
#include <QDir>

Texture::Texture()
{
}

Texture::Texture(const QString &texPath)
{    
    _texPath = texPath;
    _texImage = QImage(_texPath);

    if (_texImage.isNull()) {
        qDebug() << "Image don't load: " << _texPath;
    }
}

Texture::~Texture()
{
    if (_oglTexture != nullptr)
        _oglTexture->destroy();
    delete _oglTexture;
}

bool Texture::init()
{
    if (_isInit || _texImage.isNull())
        return true;

    _oglTexture = new QOpenGLTexture( _texImage.mirrored() );
    _oglTexture->setMaximumAnisotropy(8.0);
    _oglTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    _oglTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    _oglTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _oglTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    _oglTexture->bind();

    _isInit = true;

    return true;
}

bool Texture::isInit() const
{
    return _isInit;
}

bool Texture::bind() const
{
    if (!_isInit)
        return false;

    _oglTexture->bind();
    return true;
}

GLuint Texture::textureId() const
{
    if (_isInit)
        return _oglTexture->textureId();
    return 0;
}

bool Texture::isNull() const
{
    return _texImage.isNull();
}

