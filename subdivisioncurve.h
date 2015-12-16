#ifndef SUBDIVISIONCURVE_H
#define SUBDIVISIONCURVE_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QDebug>

class SubdivisionCurve : public QObject
{
    Q_OBJECT
public:
    explicit SubdivisionCurve(QObject *parent = 0);
    const QVector3D* getSubdivisionPoints() const;
    void calculate(const QVector<QVector3D>& controlPoints, const float &weights);
    bool getIsVisible() const;
    int subdivisionCurvePointsCount() const;
private:
    int iterations = 6;
    bool visible = true;
    QList<QVector3D> subdivisionCurvePoints;
signals:

public slots:
    void setIterations(int iterations);
    void setVisible(bool isvisible);
};

#endif // SUBDIVISIONCURVE_H
