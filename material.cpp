#include "material.h"

Material::Material()
{
    ambientTexture_id = diffuseTexture_id = specularTexture_id = 0;

    ambientColor = diffuseColor = specularColor = QColor(255, 255, 255, 255);
}

Material::~Material() {
}
