#ifndef EDGE_H
#define EDGE_H

#include <QVector3D>
#include <tuple>

class Edge
{
public:
    Edge();
    Edge(const QVector3D& firstVertex, const QVector3D& secondVertex);
    Edge& operator =(const Edge& otherEdge);
    const QVector3D& getFirstVertex() const;
    const QVector3D& getSecondVertex() const;


private:
    QVector3D firstVertex;
    QVector3D secondVertex;

};
bool operator==(const Edge& other, const Edge& o);

#endif // EDGE_H
