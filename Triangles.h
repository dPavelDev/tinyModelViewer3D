#pragma once

#include <vector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLBuffer>

struct Triangles
{
	std::vector<QVector3D> verticies, normals;
	std::vector<QVector2D> texverticies;

    QOpenGLBuffer verticiesBuffer, normalsBuffer, texverticiesBuffer;

	Triangles();
	~Triangles();

	void shrink_to_fit();
	void clear();
    void initBuffers();
    void releaseBuffers();
};

