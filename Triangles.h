#pragma once

#include <vector>
#include <QVector3D>
#include <QVector2D>

struct Triangles {
	std::vector<QVector3D> verticies, normals;
	std::vector<QVector2D> texverticies;

	Triangles();
	~Triangles();

	void shrink_to_fit();
	void clear();
};
