#include "subdivisioncurve.h"

SubdivisionCurve::SubdivisionCurve(QObject *parent) : QObject(parent)
{

}

void SubdivisionCurve::setVisible(bool isvisible)
{
    visible = isvisible;
}

void SubdivisionCurve::setIterations(int iterations)
{
    this->iterations = iterations;
}

void SubdivisionCurve::calculate(const QVector<QVector3D> &controlPoints,const float &weights)
{
    subdivisionCurvePoints.clear();
    subdivisionCurvePoints = controlPoints.toList();

    float weight = 3/4.0;
    QList<QVector3D> pts;
    qDebug() << iterations;
    for (int it = 0; it < iterations; ++it)
    {
        pts.clear();
        int size = subdivisionCurvePoints.size();

        pts.prepend( subdivisionCurvePoints[0]);
        QVector3D lastP = subdivisionCurvePoints.last();
        for (int i = 0; i < size-1; ++i)
        {
            QVector3D p1 = subdivisionCurvePoints[i] * weight  + subdivisionCurvePoints[i+1] * (1- weight);
            QVector3D p2 = subdivisionCurvePoints[i] *  (1- weight)  + subdivisionCurvePoints[i+1] *  weight;

            pts.push_back(p1);
            pts.push_back(p2);
        }

        pts.removeAt(0);
        pts.push_back(pts[0]);

        subdivisionCurvePoints = pts;
    }
}

int SubdivisionCurve::subdivisionCurvePointsCount() const
{
    return subdivisionCurvePoints.size();
}

bool SubdivisionCurve::getIsVisible() const
{
    return visible;
}

const QVector3D* SubdivisionCurve::getSubdivisionPoints() const
{
    return subdivisionCurvePoints.toVector().constData();
}

