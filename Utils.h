#include <math.h>
#include <QVector2D>
#include <QMatrix4x4>
#include <QtMath>

#pragma once

#define Pi 3.14159265358979323846264338327950288419716939937
using GLfloat = float;

typedef struct rgbCol{
	GLfloat _r;
	GLfloat _g;
	GLfloat _b;
}RGBColor;

class Utils
{
public:
	Utils();
	~Utils();
    static GLfloat DotProduct(const QVector2D &_vec1, const QVector2D &_vec2);
    static GLfloat AngleToRad(GLfloat _angle);
    static GLfloat factorial(int _num);
    static GLfloat combination(int _n, int _k);
    static int Round(GLfloat _floatnr);
    static GLfloat PointToPointDist(const QVector2D& _pt1Coords,const QVector2D& _pt2Coords);
    static GLfloat PointToVecDistance(const QVector2D &_vecpoint1, const QVector2D &_vecpoint2, const QVector2D &_point);
	
    static QMatrix4x4 rotateXmat(double_t value);
    static QMatrix4x4 rotateYmat(double_t value);
    static QMatrix4x4 rotateZmat(double_t value);

    static QMatrix4x4 centralProjectionMatrix();

    static QMatrix4x4 scaleMatrix(float factor);
    static QMatrix4x4 scaleXYMatrix(float factorX, float factorY);

    static QMatrix4x4 translate(const QVector3D& vec);

private:
};

