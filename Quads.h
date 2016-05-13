#pragma once

#include <vector>
#include <QVector2D>
#include <QVector3D>


struct Quads
{
	std::vector<QVector3D> verticies, normals;
	std::vector<QVector2D> texverticies;

	Quads();
	~Quads();

	void shrink_to_fit();
	void clear();
};

