#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include <QObject>
#include <QVector3D>
#include <QVector>
#include <QDebug>


class BSplineCurve : public QObject
{
    Q_OBJECT
public:
    explicit BSplineCurve(QObject *parent = 0);
    BSplineCurve(const QVector<QVector3D>& controlPoints);

    ~BSplineCurve();
    void calculatePoints(const QVector<QVector3D>& controlPoints);

    void recalculate();
    void draw();

    const QVector3D *getBSplinePoints() const;
    const QVector3D *getNURBSPoints() const;

    int pointsCount() const;
    int nurbsPointsCount() const;

    bool getIsVisible() const;
    bool getIsNurbsVisible() const;
    void calculateNURPoints(const QVector<QVector3D>& controlPoints, const QVector<float>& controlPointWeights);


private:
    QVector<QVector3D> bsplinePoints;
    QVector<QVector3D> nurbsPoints;

    QVector<QVector3D> controlPoints;

    QVector<float_t> csomoPontValues;
    int degree = 3;

    QVector3D getUniformPoint(float t, const QVector3D &point1, const QVector3D &point2, const QVector3D &point3, const QVector3D &point4);

    void getCsomoErteks(const QVector<QVector3D>& controlPoints);
    bool isVisible;
    bool isNurbsVisible;

signals:

public slots:
    void setVisible(bool visible);
    void setNurbsVisible(bool visible);
};

#endif // BSPLINECURVE_H
