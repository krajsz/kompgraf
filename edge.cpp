#include "edge.h"

Edge::Edge():firstVertex(0,0,0), secondVertex(0,0,0)
{
}

Edge::Edge(const QVector3D &firstVertex, const QVector3D &secondVertex): firstVertex(firstVertex), secondVertex(secondVertex)
{
}

Edge& Edge::operator =(const Edge& otherEdge)
{
    firstVertex = otherEdge.firstVertex;
    secondVertex = otherEdge.secondVertex;
    return *this;
}

bool operator==(const Edge& e, const Edge& e1)
{
    return true;
    return std::tie(e.getFirstVertex(), e.getSecondVertex()) == std::tie(e1.getFirstVertex() ,e1.getSecondVertex());
}

const QVector3D& Edge::getFirstVertex() const
{
    return this->firstVertex;
}

const QVector3D& Edge::getSecondVertex() const
{
    return this->secondVertex;
}

