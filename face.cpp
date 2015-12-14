#include "face.h"

Face::Face()
{
}

Face::Face(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3): firstVertex(v1),
                                                                                  secondVertex(v2),
                                                                                  thirdVertex(v3)
{
    vertices.push_back(firstVertex);
    vertices.push_back(secondVertex);
    vertices.push_back(thirdVertex);
}
Face& Face::operator =(const Face& other)
{
    firstVertex = other.firstVertex;
    secondVertex = other.secondVertex;
    thirdVertex = other.thirdVertex;

    vertices.clear();
    vertices.push_back(firstVertex);
    vertices.push_back(secondVertex);
    vertices.push_back(thirdVertex);

    return *this;
}

bool Face::containsVertex(const QVector3D &edge)
{
    return (edge == getFirstVertex()) || (edge == getSecondVertex()) || (edge == getThirdVertex());
}

bool operator ==(const Face& f, const Face& f1)
{
    return std::tie(f.getFirstVertex(), f.getSecondVertex(), f.getThirdVertex())
            == std::tie(f1.getFirstVertex(), f1.getSecondVertex(), f1.getThirdVertex());
}

bool operator !=(const Face& f, const Face& f1)
{
    return !(f == f1);
}

const QList<QVector3D>& Face::getVertices()
{
    return vertices;
}

const QVector3D& Face::getFirstVertex() const
{
    return firstVertex;
}

const QVector3D& Face::getSecondVertex() const
{
    return secondVertex;
}

const QVector3D& Face::getThirdVertex() const
{
    return thirdVertex;
}

void Face::setSubdivided(bool subdivided)
{
    this->subdivided = subdivided;
}


