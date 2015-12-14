#include "Utils.h"

Utils::Utils()
{
}

Utils::~Utils()
{
}
GLfloat Utils::combination(int _n, int _k)
{
    return factorial(_n) /  (factorial( _n - _k )* factorial(_k) );
}
GLfloat Utils::factorial(int _num)
{
    float result = 1;
    for (int i = 1; i <= _num; i++)
    {
        result *= i;
    }

    return result;
}

QMatrix4x4 Utils::rotateXmat(double_t value)
{
    return QMatrix4x4(
                1.0, 0, 0, 0,
                0, cos(qDegreesToRadians(value)), -sin(qDegreesToRadians(value)), 0,
                0, sin(qDegreesToRadians(value)), cos(qDegreesToRadians(value)), 0,
                0, 0, 0, 1.0);
}

QMatrix4x4 Utils::rotateZmat(double_t value)
{
    return QMatrix4x4(cos(qDegreesToRadians(value)), -sin(qDegreesToRadians(value)), 0, 0,
                      sin(qDegreesToRadians(value)), cos(qDegreesToRadians(value)), 0, 0,
                      0, 0, 1.0, 0,
                      0, 0, 0, 1.0);
}

QMatrix4x4 Utils::rotateYmat(double_t value)
{
    return QMatrix4x4(cos(qDegreesToRadians(value)), 0,sin(qDegreesToRadians(value)), 0,
                      0, 1.0, 0, 0,
                      -sin(qDegreesToRadians(value)), 0, cos(qDegreesToRadians(value)), 0,
                      0, 0, 0, 1.0
                      );
}

QMatrix4x4 Utils::centralProjectionMatrix()
{
    return QMatrix4x4 (
                1 ,0, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0,
                0, 0, -1/250.0, 1);
}

QMatrix4x4 Utils::scaleMatrix(float factor)
{
    return QMatrix4x4(factor, 0, 0, 0,
                      0, factor, 0, 0,
                      0, 0, factor, 0,
                      0, 0, 0,   1);
}

QMatrix4x4 Utils::scaleXYMatrix(float factorX, float factorY)
{
    return QMatrix4x4(factorX, 0, 0, 0,
                      0, factorY, 0, 0,
                      0, 0,  1, 0,
                      0, 0, 0,   1);
}


QMatrix4x4 Utils::translate(const QVector3D &vec)
{
    return QMatrix4x4(1, 0, 0, vec.x(),
                      0, 1, 0, vec.y(),
                      0, 0, 1, vec.z(),
                      0, 0, 0, 1);
}

GLfloat Utils::AngleToRad(GLfloat _angle)
{
    return  (_angle * 180.0f) / Pi;
}
int Utils::Round(GLfloat _floatnr)
{
    return std::floor(_floatnr + 0.5);
}

GLfloat Utils::PointToPointDist(const QVector2D &_pt1Coords, const QVector2D &_pt2Coords)
{
    return std::sqrt( std::pow((_pt2Coords.x() - _pt1Coords.x()),2)+ std::pow((_pt2Coords.y()- _pt1Coords.y()),2));
}

GLfloat Utils::DotProduct(const QVector2D &_vec1, const QVector2D &_vec2)
{
    return _vec1.x()*_vec2.x() + _vec1.y()*_vec2.y();
}

GLfloat Utils::PointToVecDistance(const QVector2D &_vecpoint1, const QVector2D &_vecpoint2, const QVector2D &_point)
{
    GLfloat a = _vecpoint2.x() - _vecpoint1.x();
    GLfloat	b = _vecpoint2.y() - _vecpoint1.y();

    GLfloat c = a* (_vecpoint1.y() - _point.y()) - b* (_vecpoint1.x() - _point.x());

    GLfloat dist = c / std::sqrt(a*a + b*b);

    return dist;
}
