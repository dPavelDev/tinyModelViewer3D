#include "Quads.h"

Quads::Quads()
{
	verticies.clear();
	normals.clear();
	texverticies.clear();
}

void Quads::shrink_to_fit()
{
	verticies.shrink_to_fit();
	normals.shrink_to_fit();
	texverticies.shrink_to_fit();
}

void Quads::clear()
{
	verticies.clear();
	normals.clear();
	texverticies.clear();
}

Quads::~Quads() {}
