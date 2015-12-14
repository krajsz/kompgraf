#include "face.h"

Face::Face()
{
}

Face::Face(const Edge &firstEdge, const Edge &secondEdge, const Edge &thirdEdge): firstEdge(firstEdge),
                                                                                  secondEdge(secondEdge),
                                                                                  thirdEdge(thirdEdge)
{
}
Face& Face::operator =(const Face& other)
{
    firstEdge = other.firstEdge;
    secondEdge = other.secondEdge;
    thirdEdge = other.thirdEdge;
    return *this;
}

bool Face::containsEdge(const Edge &edge)
{
    return (edge == getFirstEdge()) || (edge == getSecondEdge()) || (edge == getThirdEdge());
}

bool operator ==(const Face& f, const Face& f1)
{
    return std::tie(f.getFirstEdge(), f.getSecondEdge(), f.getThirdEdge())
            == std::tie(f1.getFirstEdge(), f1.getSecondEdge(), f1.getThirdEdge());
}

bool operator !=(const Face& f, const Face& f1)
{
    return !(f == f1);
}

const Edge& Face::getFirstEdge() const
{
    return firstEdge;
}

const Edge& Face::getSecondEdge() const
{
    return secondEdge;
}

const Edge& Face::getThirdEdge() const
{
    return thirdEdge;
}



