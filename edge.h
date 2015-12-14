#ifndef EDGE_H
#define EDGE_H

#include <QVector3D>

class Edge
{
public:
    Edge();
    Edge(const QVector3D& firstPoint, const QVector3D& secondPoint);
    Edge& operator =(const Edge& otherEdge);

    bool operator ==(const Edge& other);

private:
    QVector3D firstPoint;
    QVector3D secondPoint;



};

#endif // EDGE_H
