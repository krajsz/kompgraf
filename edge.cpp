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
    return std::tie(e.getFirstVertex(), e.getSecondVertex())
            == std::tie(e1.getFirstVertex() ,e1.getSecondVertex());
}

void Edge::addFace(const Face &face)
{
    faces.push_back(face);
}

bool Edge::inFace( Face& face)
{
    int ccount = 0;
    for (int i = 0; i < face.getVertices().size(); ++i)
    {
        if ((face.getVertices()[i] == firstVertex) || (face.getVertices()[i] == secondVertex))
        {
            ccount++;
        }
        if (ccount == 2)
        {
           // qDebug() << "contains!";
            break;
        }
    }
    return ccount == 2;
}

const QVector3D& Edge::getFirstVertex() const
{
    return this->firstVertex;
}

const QVector3D& Edge::getSecondVertex() const
{
    return this->secondVertex;
}

void Edge::setFirstFace(const Face &face)
{
    firstFace = face;
}

void Edge::setSecondFace(const Face &face)
{
    secondFace = face;
}

const Face& Edge::getFirstFace() const
{
    return firstFace;
}

const Face& Edge::getSecondFace() const
{
    return secondFace;
}
