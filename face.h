#ifndef FACE_H
#define FACE_H
#include "edge.h"
#include <tuple>

class Face
{
public:
    Face();
    Face(const Edge& firstEdge, const Edge& secondEdge, const Edge& thirdEdge);
    Face& operator= (const Face& other);


    const Edge& getFirstEdge() const;
    const Edge& getSecondEdge() const;
    const Edge& getThirdEdge() const;

    bool containsEdge(const Edge& edge) ;
private:

    Edge firstEdge;
    Edge secondEdge;
    Edge thirdEdge;
};
bool operator ==(const Face& f, const Face& f1);
bool operator != (const Face& f, const Face& f1);

#endif // FACE_H
