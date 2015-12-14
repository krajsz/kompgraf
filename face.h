#ifndef FACE_H
#define FACE_H
#pragma once

#include <tuple>
#include <QList>
#include <QVector3D>

class Face
{
public:
    Face();
    Face(const QVector3D& firstEdge, const QVector3D& secondEdge, const QVector3D& thirdEdge);
    Face& operator= (const Face& other);

    const QList<QVector3D> &getVertices();

    const QVector3D& getFirstVertex() const;
    const QVector3D& getSecondVertex() const;
    const QVector3D& getThirdVertex() const;

    void setSubdivided(bool subdivided);
    bool containsVertex(const QVector3D& edge) ;
private:

    bool subdivided = false;
    QVector3D firstVertex;
    QVector3D secondVertex;
    QVector3D thirdVertex;

    QList<QVector3D> vertices;
};
bool operator ==(const Face& f, const Face& f1);
bool operator != (const Face& f, const Face& f1);

#endif // FACE_H
