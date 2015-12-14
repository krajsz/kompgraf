#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QDebug>
#include "Utils.h"
#include <math.h>
#include <QOpenGLFunctions>

class BezierCurve : public QObject
{
    Q_OBJECT
public:
    explicit BezierCurve(QObject *parent = 0);
    ~BezierCurve();
    void calculatePoints(const QVector<QVector3D>& controlPoints);
    void calculateRationalPoints(const QVector<QVector3D>& controlPoints, const QVector<float> &weights);
    const QVector3D *getBezierPoints() const;
    const QVector3D *getRationalBezierPoints() const;

    void draw();


    const QVector3D& getAt(size_t index) const;
    bool getIsVisible() const;
    bool getIsRationalVisible() const;
    int pointsCount() const;
    int rationalPointsCount() const;

private:
    QVector<QVector3D> bezierPoints;
    QVector<QVector3D> rationalBezierPoints;

    float bernsteinValue(int n, int i, float t);

    bool isVisible;
    bool isRationalVisible;

signals:

public slots:
    void setVisible(bool visible);
    void setRationalVisible(bool visible);

};

#endif // BEZIERCURVE_H
