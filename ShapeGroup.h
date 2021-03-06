#pragma once

#include "Material.h"
#include "Triangles.h"
#include "Quads.h"

struct ShapeGroup
{
    Triangles triangles;
    Quads quads;

    Material material;

    ShapeGroup();
    ~ShapeGroup();

    void clear();
};

