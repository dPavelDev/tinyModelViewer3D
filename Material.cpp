#include "Material.h"

Material::Material()
{
    ambientColor = diffuseColor = specularColor = emissionColor = transmissionFilter = QColor(255, 255, 255, 255);
}

Material::~Material()
{
}

