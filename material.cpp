#include "material.h"

Material::Material()
{
	ambientColor = diffuseColor = specularColor = QColor(255, 255, 255, 255);
}

Material::~Material()
{
}

