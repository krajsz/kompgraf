#ifndef BSPLINESURFACE_H
#define BSPLINESURFACE_H

#include <QMatrix4x4>
#include <QVector>
#include <QtMath>
#include "Utils.h"


class BSplineSurface : public QObject
{
    Q_OBJECT
public:
    BSplineSurface(QObject* parent);
    void init();

    const QVector<QVector3D>& getSurfaceVerPoints(int i) const;
    const QVector<QVector3D>& getSurfaceHorPoints(int i) const;
    const QVector<QVector<QVector3D>>& getGridPoints() const;
    const QVector3D* getTrianglePoints() const;
    const QVector3D* getTriangleFaceNormals() const;
    const QVector3D* getTriangleVertexNormals() const;

    int getHorizontalBsplinesCount() const;
    int getVerticalBsplinesCount() const;
    int getTriangleNormalsCount() const;
    int getTriangleVertexNormalsCount() const;
    int getDensity() const;


    QVector<QVector<QVector3D> > translateSurfaceControlPoints(const QVector<QVector<QVector3D>>& points);

    const QMatrix4x4& getTransformationMatrix() const;

    int pointsCount()   const;
    int trianglesCount() const;
    bool getIsVisible() const;
    bool getIsShadedVisible() const;
    bool getIsControlPointsVisible() const;

    void updateRotation(float_t rotX, float_t rotZ);

    void calculate(const QVector<QVector<QVector3D>>& surfaceControlPoints,
                                   const QVector<QVector<float_t> > &weights, int density);

    float_t getXrotation() const;
    float_t getZrotation() const;

private:
    QVector<QVector3D> trianglePoints;
    QVector<QVector3D> triangleFaceNormals;
    QVector<QVector3D> triangleVertexNormals;

    int density;

    QVector<QVector<QVector3D>> gridPoints;

    float_t xRotation =  -50.0;
    float_t zRotation =  30.0;

    QMatrix4x4 transformationMatrix;
    QMatrix4x4 wtvMatrix;

    QMatrix4x4 currentRotation;

    QVector<QVector<float_t>> Ns;

    QVector<QVector<QVector3D>> surfacePointsVer;
    QVector<QVector<QVector3D>> surfacePointsHor;

    int degree = 3;
    QVector<float_t> csomoPontValues = {0, 0, 0, 0, 1, 1, 1, 1};

    bool isVisible;
    bool isControlNetVisible ;
    bool isShadedVisible;
signals:

public slots:
    void setVisible(bool visible);
    void setControlNetVisible(bool visible);
    void setShadedVisible(bool visible);

};

#endif // BSPLINESURFACE_H
