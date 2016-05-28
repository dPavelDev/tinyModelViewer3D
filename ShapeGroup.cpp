#include "ShapeGroup.h"


ShapeGroup::ShapeGroup()
{
    //clear();
}

void ShapeGroup::clear()
{
    triangles.clear();
    quads.clear();
    material = Material();
}

ShapeGroup::~ShapeGroup()
{
}

