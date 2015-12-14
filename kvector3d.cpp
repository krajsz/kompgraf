#include "kvector3d.h"

KVector3D::KVector3D():QVector3D(0,0,0)
{
}

void KVector3D::setWeight(float _weight)
{
    weight = _weight;
}

const float KVector3D::getWeight()
{
    return weight;
}

KVector3D::~KVector3D()
{
}

KVector3D::KVector3D(const QVector3D &vec3)
{
    setX(vec3.x());
    setY(vec3.y());
    setZ(vec3.z());
}


