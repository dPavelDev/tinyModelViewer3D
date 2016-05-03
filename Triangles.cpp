#include "Triangles.h"


void Triangles::shrink_to_fit()
{
	verticies.shrink_to_fit();
	normals.shrink_to_fit();
	texverticies.shrink_to_fit();
}

void Triangles::clear()
{
	verticies.clear();
	normals.clear();
	texverticies.clear();
}

Triangles::Triangles()
{
	verticies.clear();
	normals.clear();
	texverticies.clear();
}

Triangles::~Triangles()
{
}
