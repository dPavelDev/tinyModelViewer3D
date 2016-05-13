#include "ShapeGroup.h"


ShapeGroup::ShapeGroup()
{
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

