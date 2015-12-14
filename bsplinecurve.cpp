#include "bsplinecurve.h"

BSplineCurve::BSplineCurve(QObject *parent) : QObject(parent),
        isVisible(true),
        isNurbsVisible(true)
{

}
BSplineCurve::~BSplineCurve()
{
}

int BSplineCurve::pointsCount() const
{
    return bsplinePoints.size();
}

void BSplineCurve::setVisible(bool visible)
{
    isVisible = visible;
}

void BSplineCurve::draw()
{

}

const QVector3D *BSplineCurve::getBSplinePoints() const
{
    return bsplinePoints.constData();
}

const QVector3D* BSplineCurve::getNURBSPoints() const
{
    return nurbsPoints.constData();
}

bool BSplineCurve::getIsNurbsVisible() const
{
    return isNurbsVisible;
}

void BSplineCurve::setNurbsVisible(bool visible)
{
    isNurbsVisible = visible;
}

int BSplineCurve::nurbsPointsCount() const
{
    return nurbsPoints.size();
}

void BSplineCurve::calculatePoints(const QVector<QVector3D> &controlPoints)
{
    bsplinePoints.clear();
    if (controlPoints.size() > 3)
    {
        QVector3D calculatedPoint;
        for(float t = 0; t <=1.0; t+=0.01)
        {
            calculatedPoint = getUniformPoint(t, controlPoints.at(0), controlPoints.at(0), controlPoints.at(0), controlPoints.at(0));
            bsplinePoints.push_back(calculatedPoint);
        }

        for(float t = 0; t <=1.0; t+=0.01)
        {
            calculatedPoint = getUniformPoint(t, controlPoints.at(0), controlPoints.at(0), controlPoints.at(0), controlPoints.at(1));
            bsplinePoints.push_back(calculatedPoint);
        }

        for(float t = 0; t <=1.0; t+=0.01)
        {
            calculatedPoint = getUniformPoint(t, controlPoints.at(0), controlPoints.at(0), controlPoints.at(1), controlPoints.at(2));

            bsplinePoints.push_back(calculatedPoint);
        }

        for (size_t i = 0; i < controlPoints.size() - 3; ++i)
        {
            for(float t = 0; t <=1.0; t+=0.01)
            {
                calculatedPoint = getUniformPoint(t, controlPoints.at(i), controlPoints.at(i+1), controlPoints.at(i+2), controlPoints.at(i+3));

                bsplinePoints.push_back(calculatedPoint);
            }
        }

        for(float t = 0; t <=1.0; t+=0.01)
        {
            calculatedPoint = getUniformPoint(t, controlPoints.at(controlPoints.size()-3), controlPoints.at(controlPoints.size()-2),
                                                 controlPoints.at(controlPoints.size()-1), controlPoints.at(controlPoints.size()-1));

            bsplinePoints.push_back(calculatedPoint);
        }
        for(float t = 0; t <=1.0; t+=0.01)
        {
            calculatedPoint = getUniformPoint(t, controlPoints.at(controlPoints.size()-2), controlPoints.at(controlPoints.size()-1),
                                                 controlPoints.at(controlPoints.size()-1), controlPoints.at(controlPoints.size()-1));
            bsplinePoints.push_back(calculatedPoint);
        }
        for(float t = 0; t <=1.0; t+=0.01)
        {
            calculatedPoint = getUniformPoint(t, controlPoints.at(controlPoints.size()-1), controlPoints.at(controlPoints.size()-1),
                                                 controlPoints.at(controlPoints.size()-1), controlPoints.at(controlPoints.size()-1));
            bsplinePoints.push_back(calculatedPoint);
        }
    }
}

QVector3D BSplineCurve::getUniformPoint(float t, const QVector3D &point1, const QVector3D &point2, const QVector3D &point3, const QVector3D &point4)
{
    float tPow2 = pow(t, 2);
    float tPow3 = pow(t, 3);

    QVector3D uniformPoint;
    uniformPoint+= (1/6.0 - 0.5*t + 0.5*tPow2 - (1/6.0)*tPow3)*point1;
    uniformPoint+= (0.5 * tPow3 - tPow2 + 2/3.0) * point2;
    uniformPoint+= (-0.5 * tPow3 + 0.5 * tPow2 + 0.5 * t + 1/6.0) * point3;
    uniformPoint+= ((1/6.0) * tPow3) * point4;

    return QVector3D(uniformPoint);
}

bool BSplineCurve::getIsVisible() const
{
    return isVisible;
}

void BSplineCurve::recalculate()
{

}

void BSplineCurve::getCsomoErteks(const QVector<QVector3D> &controlPoints)
{
    csomoPontValues.clear();
    csomoPontValues.resize(degree+1);
    std::for_each(csomoPontValues.begin(), csomoPontValues.end(), [](float& val) { val = 0.0; } );

    float bsplineLength = 0.0;
    QVector<float> bsplineSegmentLengths;
    for (size_t i = 0; i < controlPoints.size() - degree; ++i)
    {
        float length = 0.0;
        for (size_t j = i; j < i + degree; ++j)
        {
            length += controlPoints[j].distanceToPoint(controlPoints[j+1]);
        }
        bsplineLength += length;
        bsplineSegmentLengths.push_back(bsplineLength);
    }
    for (size_t i = 0; i < bsplineSegmentLengths.size() - 1; ++i)
    {
        csomoPontValues.push_back(bsplineSegmentLengths[i]/bsplineLength);
    }

    for (int i = 0; i <= degree; ++i)
    {
        csomoPontValues.push_back(1.0);
    }
}

void BSplineCurve::calculateNURPoints(const QVector<QVector3D> &controlPoints, const QVector<float> &controlPointWeights)
{
    nurbsPoints.clear();
    getCsomoErteks(controlPoints);
    QVector<float> N;

    for (float u = 0.0f; u <= 1.0f; u += 0.001)
    {
        N.clear();
        N.reserve(controlPoints.size());
        N.resize(controlPoints.size());

        std::for_each(N.begin(), N.end(), [](float& value) { value = 0.0;});

        if (u == csomoPontValues.first())
        {
            N.first() = 1.0f;
        }
        else if (u == csomoPontValues.last())
        {
            N.last() = 1.0f;
        }
        else
        {
            int k;
            for (size_t i = degree; i < csomoPontValues.size() - degree; ++i)
            {
                if (u >= csomoPontValues.at(i) && u < csomoPontValues.at(i + 1))
                {
                    k = i;
                   // qDebug() << "k " << k;
                    N[k] = 1.0f;
                    break;
                }
            }
            for (int d = 1; d <= degree; ++d)
            {
                N[k - d] = ((csomoPontValues.at(k + 1) - u)/(csomoPontValues.at(k + 1) -
                                                             csomoPontValues.at(k - d + 1)))*N.at(k - d + 1);
                for (int i = k - d + 1; i <= k - 1; ++i)
                {
                    N[i] = N[i]*((u - csomoPontValues.at(i))/(csomoPontValues.at(i + d) - csomoPontValues[i]))
                         + N[i + 1]*((csomoPontValues[i + d + 1] - u)/(csomoPontValues[i + d + 1] - csomoPontValues[i + 1]));
                }
                N[k] = ((u - csomoPontValues[k])/(csomoPontValues[k + d] - csomoPontValues[k]))*N[k];
            }
        }

        QVector3D point {0, 0,0};
        float subSum = 0.0f;
        for (int i = 0; i < controlPointWeights.size(); ++i)
        {
            subSum += controlPointWeights[i]*N[i];
        }

        for (size_t i = 0; i < controlPoints.size(); ++i)
        {
            point += controlPointWeights[i]*N[i]*controlPoints[i];
        }
        nurbsPoints.push_back((1/subSum)*point);
    }
}
