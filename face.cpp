#include "face.h"

Face::Face()
{
}

Face::Face(const Edge &firstEdge, const Edge &secondEdge, const Edge &thirdEdge): firstEdge(firstEdge),
                                                                                  secondEdge(secondEdge),
                                                                                  thirdEdge(thirdEdge)
{
}

