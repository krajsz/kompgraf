#include "edge.h"

Edge::Edge():firstPoint(0,0,0), secondPoint(0,0,0)
{
}

Edge::Edge(const QVector3D &firstPoint, const QVector3D &secondPoint): firstPoint(firstPoint), secondPoint(secondPoint)
{
}

Edge& Edge::operator =(const Edge& otherEdge)
{
    firstPoint = otherEdge.firstPoint;
    secondPoint = otherEdge.secondPoint;
    return *this;
}
bool Edge::operator ==(const Edge& other)
{
    return (other.firstPoint == firstPoint) && (other.secondPoint == secondPoint);
}
