#include "bsplinesurface.h"

BSplineSurface::BSplineSurface(QObject *parent) :
    QObject(parent),
    isVisible(false),
    isShadedVisible(false),
    isControlNetVisible(false)
{
    wtvMatrix = Utils::translate(QVector3D(150, 150, -150)) * Utils::scaleXYMatrix( 150,  150) *
            Utils::translate(QVector3D(1,1,0));

    currentRotation =     currentRotation = Utils::rotateYmat(xRotation ) *
            Utils::rotateZmat(zRotation  ) ;

    transformationMatrix = wtvMatrix *currentRotation ;
}

const QMatrix4x4& BSplineSurface::getTransformationMatrix() const
{
    return this->transformationMatrix;
}

void BSplineSurface::updateRotation(float_t rotX, float_t rotZ)
{
    xRotation = rotX;
    zRotation = rotZ;

    currentRotation = Utils::rotateYmat(xRotation/  2.0) * Utils::rotateZmat(zRotation /  2.0) ;
    transformationMatrix = wtvMatrix * currentRotation  ;
}

float_t BSplineSurface::getZrotation() const
{
    return zRotation;
}

float_t BSplineSurface::getXrotation() const
{
    return xRotation;
}

void BSplineSurface::init()
{
    Ns.reserve(102);
    for (auto& i : Ns)
    {
        i.reserve(5);
    }
    QVector<float_t> N;

    for (float u = 0.0f; u <= 1.0f; u += 0.01)
    {
        N.clear();
        N.resize(4);
        N.reserve(4);

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
            for (int  i = degree; i < csomoPontValues.size() - degree; ++i)
            {
                if (u >= csomoPontValues.at(i) && u < csomoPontValues.at(i + 1))
                {
                    k = i;
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

        Ns.push_back(N);
    }
}

void BSplineSurface::calculate(const QVector<QVector<QVector3D>>& surfaceControlPoints,
                               const QVector<QVector<float_t> > &weights, int density)
{
    this->density = density;

    surfacePointsVer.clear();
    surfacePointsHor.clear();

    surfacePointsVer.reserve(density +1);
    surfacePointsHor.reserve(density +1);


    for (int i = 0; i < surfacePointsVer.size();++i)
    {
        surfacePointsVer[i].reserve(102);
        surfacePointsHor[i].reserve(102);
    }

    gridPoints.clear();
    gridPoints.reserve( std::pow(density,2));

    for (int i =0 ; i < gridPoints.size(); ++i)
    {
        gridPoints[i].reserve(std::pow(density,2));
    }

    int i =0;
    for (float_t uu = 0.0; uu<= 1.05 ; uu+= 0.05, i+=5)
    {
        int j = 0;
        QVector<QVector3D> tmpVer;
        QVector<QVector3D> tmpHor;

        tmpVer.reserve(102);
        tmpHor.reserve(102);

        for (float_t v = 0.0; v <=1.0; v+=0.01, ++j)
        {
            QVector3D pointVer =  {0, 0,0 };
            QVector3D pointHor =  {0, 0,0 };

            for (int k = 0; k < 4; ++k)
            {
                for (int l = 0; l < 4; ++l)
                {
                    pointVer += weights[k][l] * surfaceControlPoints[k][l] * Ns[i][k] * Ns[j][l];
                    pointHor += weights[k][l] * surfaceControlPoints[k][l] * Ns[j][k] * Ns[i][l];
                }
            }
            tmpHor.push_back(pointHor);
            tmpVer.push_back(pointVer);
        }
        surfacePointsHor.push_back(tmpVer);
        surfacePointsVer.push_back(tmpHor);
    }

    i =0;
    for (float_t uu = 0.0; uu<= 1.00  ; uu+= 0.01, i+=1)
    {
        int j = 0;
        QVector<QVector3D> tmp;
        tmp.reserve( static_cast<int> (1.00/ density) +1 );

        for (float_t v = 0.0; v <=1.00   ; v+=0.01, j+=1)
        {
            QVector3D point = {0, 0,0};

            for (int k = 0; k < 4; ++k)
            {
                for (int l = 0; l < 4; ++l)
                {
                    point +=  weights[k][l] * surfaceControlPoints[k][l] * Ns[j][k] * Ns[i][l];
                }
            }
            tmp.push_back(point);
        }
        gridPoints.push_back(tmp);
    }

    trianglePoints.clear();
    trianglePoints.reserve(gridPoints.size() * gridPoints.size() * 6);
    triangleFaceNormals.clear();
    triangleVertexNormals.clear();

    QVector3D n1;
    QVector3D n2;

    for (int i = 0; i < gridPoints.size()-1; ++i)
    {
        for (int j = 0; j < gridPoints[i].size()-1; ++j)
        {
            trianglePoints.push_back(gridPoints[i][j]);
            trianglePoints.push_back(gridPoints[i+1][j]);
            trianglePoints.push_back(gridPoints[i][j+1]);

            //n1 = QVector3D::crossProduct(gridPoints[i+1][j] -gridPoints[i][j], gridPoints[i][j+1]- gridPoints[i+1][j] );
            n1 = QVector3D::normal(gridPoints[i][j], gridPoints[i+1][j], gridPoints[i][j+1] );

            triangleFaceNormals.push_back(n1);
            triangleFaceNormals.push_back(n1);
            triangleFaceNormals.push_back(n1);

            trianglePoints.push_back(gridPoints[i][j+1]);
            trianglePoints.push_back(gridPoints[i+1][j]);
            trianglePoints.push_back(gridPoints[i+1][j+1]);

            //n2 = QVector3D::crossProduct(gridPoints[i+1][j] -gridPoints[i][j+1], gridPoints[i+1][j+1]- gridPoints[i][j+1] );
            n2 = QVector3D::normal(gridPoints[i][j+1], gridPoints[i+1][j], gridPoints[i+1][j+1] );

            triangleFaceNormals.push_back(n2);
            triangleFaceNormals.push_back(n2);
            triangleFaceNormals.push_back(n2);

        }
    }

    for (int i = 1; i < gridPoints.size()-2; ++i)
    {
        QVector3D N {0 , 0, 0};
        for (int j = 1; j < gridPoints[i].size()-2; ++j)
        {
            //v2-v1 v3-v1 |
            QVector3D n1 = QVector3D::crossProduct(gridPoints[i][j-1] -gridPoints[i-1][j-1],
                    gridPoints[i][j]- gridPoints[i -1][j-1] );


            QVector3D n2 = QVector3D::crossProduct(gridPoints[i][j] -gridPoints[i-1][j-1],
                    gridPoints[i-1][j]- gridPoints[i-1][j-1] );

            QVector3D n3 = QVector3D::crossProduct(gridPoints[i][j] -gridPoints[i-1][j],
                    gridPoints[i][j+1]- gridPoints[i-1][j] );

            QVector3D n4 = QVector3D::crossProduct(gridPoints[i+1][j] -gridPoints[i][j],
                    gridPoints[i+1][j+1]- gridPoints[i][j] );

            QVector3D n5 = QVector3D::crossProduct(gridPoints[i+1][j+1] -gridPoints[i][j],
                    gridPoints[i][j+1]- gridPoints[i ][j] );


            QVector3D n6 = QVector3D::crossProduct(gridPoints[i+1][j] -gridPoints[i][j-1],
                    gridPoints[i][j]- gridPoints[i ][j-1] );

            //N+=n1;
            N+=n2;
            //N+=n3;
            N+=n4;
            //N+=n5;
            N+=n6;

            triangleVertexNormals.push_back(N);
            triangleVertexNormals.push_back(N);
            triangleVertexNormals.push_back(N);

            triangleVertexNormals.push_back(N);
            triangleVertexNormals.push_back(N);
            triangleVertexNormals.push_back(N);

        }
    }
}

int BSplineSurface::getDensity() const
{
    return density;
}

int BSplineSurface::getTriangleVertexNormalsCount() const
{
    return triangleVertexNormals.size();
}

const QVector3D* BSplineSurface::getTriangleVertexNormals() const
{
    return triangleVertexNormals.constData();
}

const QVector3D* BSplineSurface::getTriangleFaceNormals() const
{
    return triangleFaceNormals.constData();
}

void BSplineSurface::setShadedVisible(bool visible)
{
    isShadedVisible = visible;
}

int BSplineSurface::getTriangleNormalsCount() const
{
    return triangleFaceNormals.size();
}

bool BSplineSurface::getIsShadedVisible() const
{
    return isShadedVisible;
}

int BSplineSurface::trianglesCount() const
{
    return trianglePoints.size();
}

const QVector3D* BSplineSurface::getTrianglePoints() const
{
    return trianglePoints.constData();
}

const QVector<QVector<QVector3D>>& BSplineSurface::getGridPoints() const
{
    return gridPoints;
}

QVector<QVector<QVector3D>> BSplineSurface::translateSurfaceControlPoints(const QVector<QVector<QVector3D> > &points)
{
    QVector<QVector<QVector3D>> tmp;

    for (int i = 0; i < points.size(); ++i)
    {
        QVector<QVector3D> tmpVec;
        QVector3D point;
        for (int j = 0; j < points[i].size(); ++j)
        {
            point =  transformationMatrix * points[i][j];
            tmpVec.push_back(point);
        }
        tmp.push_back(tmpVec);
    }
    return tmp;
}

bool BSplineSurface::getIsControlPointsVisible() const
{
    return isControlNetVisible;
}

int BSplineSurface::getHorizontalBsplinesCount() const
{
    return surfacePointsVer.size();
}

void BSplineSurface::setControlNetVisible(bool visible)
{
    this->isControlNetVisible = visible;
}

int BSplineSurface::getVerticalBsplinesCount() const
{
    return surfacePointsHor.size();
}

const QVector<QVector3D> &BSplineSurface::getSurfaceHorPoints(int i) const
{
    return surfacePointsVer.at(i);
}

const QVector<QVector3D> &BSplineSurface::getSurfaceVerPoints(int i) const
{
    return surfacePointsHor.at(i);
}

bool BSplineSurface::getIsVisible() const
{
    return this->isVisible;
}

int BSplineSurface::pointsCount() const
{
    return surfacePointsHor.at(0).size();
}

void BSplineSurface::setVisible(bool visible)
{
    this->isVisible = visible;
}
