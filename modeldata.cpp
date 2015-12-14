#include "modeldata.h"

ModelData::ModelData()
{
}

ModelData& ModelData::operator =(const ModelData& other)
{
    this->normals = other.normals;
    this->vertices = other.vertices;
    this->normalIndices = other.normalIndices;
    this->vertexIndices = other.vertexIndices;
    return *this;
}

QList<unsigned int>& ModelData::getNormalIndices()
{
    return this->normalIndices;
}

bool ModelData::hasData() const
{
    return vertices.size() > 0;
}

QList<unsigned int>& ModelData::getVertexIndices()
{
    return this->vertexIndices;
}

QList<QVector3D>& ModelData::getNormals()
{
    return this->normals;
}

QList<QVector3D>& ModelData::getVertices()
{
    return this->vertices;
}

void ModelData::setNormalIndices(const QList<unsigned int> &normalIndices)
{
    this->normalIndices = normalIndices;
}

void ModelData::setVertexIndices(const QList<unsigned int> &vertexIndices)
{
    this->vertexIndices = vertexIndices;
}

void ModelData::setNormals(const QList<QVector3D> &normals)
{
    this->normals = normals;
}

void ModelData::setVertices(const QList<QVector3D> &vertices)
{
    this->vertices = vertices;
}
