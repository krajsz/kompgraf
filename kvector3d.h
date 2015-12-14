#ifndef KVECTOR3D_H
#define KVECTOR3D_H

#include <QVector3D>
#include <QPoint>
#include <QString>

class KVector3D : public QVector3D
{
public:
    KVector3D();
    KVector3D(const QVector3D& vec3);
    ~KVector3D();

    void setWeight(float _weight);
    const float getWeight();

private:
    float weight;
};

#endif // KVECTOR3D_H
