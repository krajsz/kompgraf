#include "beziercurve.h"

BezierCurve::BezierCurve(QObject *parent) : QObject(parent),
                isVisible(true),
                isRationalVisible(true)
{
}

BezierCurve::~BezierCurve()
{
}
const QVector3D &BezierCurve::getAt(size_t index) const
{
    return bezierPoints.at(index);
}

void BezierCurve::draw()
{

}

int BezierCurve::pointsCount() const
{
    return bezierPoints.size();
}

void BezierCurve::setVisible(bool visible)
{
    isVisible = visible;
}

float BezierCurve::bernsteinValue(int n, int i, float t)
{
    return Utils::combination(n, i) * pow(t, i) * pow(1 - t, n -i);
}

void BezierCurve::calculatePoints(const QVector<QVector3D> &controlPoints)
{
    bezierPoints.clear();

    for (float t = 0; t<=1; t+= 0.01)
    {
        QVector3D tmpfromKvec3{0,0,0};
        for (int i = 0; i < controlPoints.size(); ++i)
        {
            tmpfromKvec3 +=controlPoints[i] * bernsteinValue(controlPoints.size() - 1, i, t);
        }
        bezierPoints.push_back(tmpfromKvec3);
    }
}

int BezierCurve::rationalPointsCount() const
{
    return rationalBezierPoints.size();
}

void BezierCurve::setRationalVisible(bool visible)
{
    isRationalVisible = visible;
}

bool BezierCurve::getIsRationalVisible() const
{
    return this->isRationalVisible;
}

const QVector3D *BezierCurve::getBezierPoints() const
{
    return bezierPoints.constData();
}

const QVector3D* BezierCurve::getRationalBezierPoints() const
{
    return rationalBezierPoints.constData();
}

bool BezierCurve::getIsVisible() const
{
    return isVisible;
}

void BezierCurve::calculateRationalPoints(const QVector<QVector3D> &controlPoints, const QVector<float> &weights)
{

    rationalBezierPoints.clear();
    QVector3D point { 0,0,0};
    QVector<float_t> bernsteinValues;
    for (float t = 0; t <= 1.0; t+=0.01)
    {
        bernsteinValues.clear();
        point.setX(0);
        point.setY(0);
        point.setZ(0);

        for (int i = 0; i < controlPoints.size(); ++i)
        {
            bernsteinValues.push_back(bernsteinValue(controlPoints.size()- 1, i, t));
            point+= weights.at(i) * controlPoints.at(i) * bernsteinValues.back();
        }

        float subsum = 0.0;
        for (int i = 0; i < weights.size(); ++i)
        {
            subsum += weights.at(i) * bernsteinValues.at(i);
        }

        point*= 1/subsum;
        rationalBezierPoints.push_back(point);
    }
}
