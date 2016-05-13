#include "SceneStat.h"

SceneStat::SceneStat()
{
}

int SceneStat::materialsCount() const
{
	return _materialsCount;
}

void SceneStat::setMaterialsCount(int materialsCount)
{
	_materialsCount = materialsCount;
}

void SceneStat::incMaterialsCount(int materialsCount)
{
	_materialsCount += materialsCount;
}

int SceneStat::texturesCount() const
{
	return _texturesCount;
}

void SceneStat::setTexturesCount(int texturesCount)
{
	_texturesCount = texturesCount;
}

void SceneStat::incTexturesCount(int texturesCount)
{
	_texturesCount += texturesCount;
}

int SceneStat::verticiesCount() const
{
	return _verticiesCount;
}

void SceneStat::setVerticiesCount(int verticiesCount)
{
	_verticiesCount = verticiesCount;
}

void SceneStat::incVerticiesCount(int verticiesCount)
{
	_verticiesCount += verticiesCount;
}

int SceneStat::texVerticiesCount() const
{
	return _texVerticiesCount;
}

void SceneStat::setTexVerticiesCount(int texVerticiesCount)
{
	_texVerticiesCount = texVerticiesCount;
}

void SceneStat::incTexVerticiesCount(int texVerticiesCount)
{
	_texVerticiesCount += texVerticiesCount;
}

int SceneStat::normalsCount() const
{
	return _normalsCount;
}

void SceneStat::setNormalsCount(int normalsCount)
{
	_normalsCount = normalsCount;
}

void SceneStat::incNormalsCount(int normalsCount)
{
	_normalsCount += normalsCount;
}

int SceneStat::facesCount() const
{
	return _facesCount;
}

void SceneStat::setFacesCount(int facesCount)
{
	_facesCount = facesCount;
}

void SceneStat::incFacesCount(int facesCount)
{
	_facesCount += facesCount;
}

bool SceneStat::freeFormDetected() const
{
	return _freeFormDetected;
}

void SceneStat::setFreeFormDetected(bool freeFormDetected)
{
	_freeFormDetected = freeFormDetected;
}

bool SceneStat::isLoaded() const
{
    return _isLoaded;
}

void SceneStat::setReadBytes(long long bytes)
{
    _readBytes = bytes;
}

long long SceneStat::readBytes() const
{
    return _readBytes;
}

long long SceneStat::totalBytes() const
{
    return _totalBytes;
}

void SceneStat::setTotalBytes(long long totalBytes)
{
    _totalBytes = totalBytes;
}

void SceneStat::setCanceled()
{
    _isCanceled = true;
}

bool SceneStat::isCanceled() const
{
    return _isCanceled;
}

