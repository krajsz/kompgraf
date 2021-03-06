#ifndef EDGE_H
#define EDGE_H

#pragma once

#include <QVector3D>
#include <QDebug>
#include <tuple>
#include "face.h"

class Edge
{
public:
    Edge();
    Edge(const QVector3D& firstVertex, const QVector3D& secondVertex);
    Edge& operator =(const Edge& otherEdge);
    const QVector3D& getFirstVertex() const;
    const QVector3D& getSecondVertex() const;

    void setFirstFace(const Face& face);
    void setSecondFace(const Face& face);

    const Face& getFirstFace() const;
    const Face& getSecondFace() const;

    void addFace(const Face& face);
    bool inFace( Face &face);

private:
    QVector3D firstVertex;
    QVector3D secondVertex;

    QList<Face> faces;

    Face firstFace;
    Face secondFace;

};
bool operator==(const Edge& other, const Edge& o);

#endif // EDGE_H
