#ifndef FACE_H
#define FACE_H
#include "edge.h"

class Face
{
public:
    Face();
    Face(const Edge& firstEdge, const Edge& secondEdge, const Edge& thirdEdge);

private:

    Edge firstEdge;
    Edge secondEdge;
    Edge thirdEdge;
};

#endif // FACE_H
