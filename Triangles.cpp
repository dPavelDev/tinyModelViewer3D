#include "Triangles.h"

#include <GL/gl.h>

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

    releaseBuffers();
}

void Triangles::initBuffers()
{
    verticiesBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    verticiesBuffer.create();
    verticiesBuffer.bind();
    verticiesBuffer.allocate(&verticies[0], verticies.size() * sizeof(QVector3D));
    verticiesBuffer.release();

    normalsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    normalsBuffer.create();
    normalsBuffer.bind();
    normalsBuffer.allocate(&normals[0], normals.size() * sizeof(QVector3D));
    normalsBuffer.release();

    texverticiesBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    texverticiesBuffer.create();
    texverticiesBuffer.bind();
    texverticiesBuffer.allocate(&texverticies[0], texverticies.size() * sizeof(QVector2D));
    texverticiesBuffer.release();
}

Triangles::Triangles()
{
	verticies.clear();
	normals.clear();
	texverticies.clear();
}

Triangles::~Triangles()
{
    releaseBuffers();
}

void Triangles::releaseBuffers() {
    verticiesBuffer.release();
    texverticiesBuffer.release();
    normalsBuffer.release();
}

