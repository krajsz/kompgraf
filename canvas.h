#ifndef CANVAS_H
#define CANVAS_H
#include "bsplinecurve.h"
#include "beziercurve.h"
#include "bsplinesurface.h"
#include "leaplistener.h"
#include "subdivisioncurve.h"
#include "sceneobject.h"

#include <Leap.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QVector3D>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMatrix4x4>
#include <QUrl>
#include <typeinfo>

class Canvas Q_DECL_FINAL: public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:

    Canvas(QWidget* parent = 0 );
    ~Canvas();
    QString& getVertexShaderSource() ;
    QString& getFragmentShaderSource() ;
    QString& getGeometryShaderSource() ;

    SubdivisionCurve* getSubdivisionCurve() const;

    int getControlPointsCount() const;

private:
    int radiobuttonMode;

    QOpenGLShader* instantiateShader(QOpenGLShader *shader, const QString &filename, QOpenGLShader::ShaderTypeBit type);

    QColor backgroundColor {34 , 36, 51};
    QColor bezierCurveColor {255, 0 ,0};
    QColor bsplineCurveColor { 255, 100, 20};
    QColor nurbsColor {100, 200, 100};
    QColor rationalBezierCurveColor {0, 100, 255};
    QColor controlPointsColor { 100, 100,100};
    QColor bsplineSurfaceColor {200, 140, 25 };
    QColor bsplineSurfaceCnetColor {255, 20, 25 };
    QColor bsplineSurfaceCpointsColor {0, 20, 25 };

    LeapListener* listener;

    SceneObject* suzanne;

    Leap::Controller controller;
    Leap::Vector handDirection;
    Leap::Vector handVelocity;

    BezierCurve* bezierCurve;
    BSplineCurve* bSplineCurve;
    BSplineSurface* bSplineSurface;
    SubdivisionCurve* subdivisionCurve;

    QMatrix4x4 mProjectionMatrix;
    QMatrix4x4 mModelMatrix;

    int mModelViewProjectionMatrixLocation;
    int mModelViewProjectionMatrixLocationCurvesProgram;

    int mModelViewProjectionMatrixLocationControlNetProgram;

    int mNumControlPointsLocation;
    int mControlPointsLocation;
    int mColorLocation;
    int mColorLocationCurvesProgram;

    int mLightDirectionLocation;
    int mModelMatrixLocation;
    int mModelMatrixLocationControlNetProgram;

    void getUniformLocation(QOpenGLShaderProgram *program, int& location, const char* name);

    void recalculateSurface();
    void reinitialiseModel();

    QOpenGLShaderProgram* mCurvesProgram;
    QOpenGLShaderProgram* mShaderProgram;
    QOpenGLShaderProgram* mComputeShaderProgram;
    QOpenGLShaderProgram* mSurfaceNetProgram;
    QOpenGLShaderProgram* mGouraudShaderProgram;
    QOpenGLShaderProgram* mFlatShaderProgram;


    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mNormalBuffer;
    QOpenGLBuffer mIndexBuffer;

    QOpenGLVertexArrayObject mControlPointsVAO;
    QOpenGLVertexArrayObject mBezierCurveVAO;
    QOpenGLVertexArrayObject mBSplineCurveVAO;
    QOpenGLVertexArrayObject mNurbsCurveVAO;
    QOpenGLVertexArrayObject mRationalBezierCurveVAO;
    QOpenGLVertexArrayObject mBSplineSurfaceVerticalLinesVAO;
    QOpenGLVertexArrayObject mBSplineSurfaceHorizontalLinesVAO;
    QOpenGLVertexArrayObject mSubdivisionCurveVAO;

    QOpenGLVertexArrayObject mBSplineSurfaceCpVerticalVAO;
    QOpenGLVertexArrayObject mBSplineSurfaceCpHorizontalVAO;
    QOpenGLVertexArrayObject mBSplineSurfaceCpVAO;
    QOpenGLVertexArrayObject mBSplineSurfaceCnetVAO;
    QOpenGLVertexArrayObject mBSplineSurfaceShadedVAO;
    QOpenGLVertexArrayObject mBSplineSurfaceGouraudShadedVAO;

    QOpenGLVertexArrayObject mTriangleNormals;

    QOpenGLShader* surfaceNetFragmentShader;

    QOpenGLShader* curvesVertexShader;
    QOpenGLShader* curvesFragmentShader;
    QOpenGLShader* curvesGeometryShader;

    QOpenGLShader* computeShader;

    QOpenGLShader* surfaceFlatShadedVertexShader;
    QOpenGLShader* surfaceFlatShadedFragmentShader;

    QOpenGLShader* surfaceGouraudShadedVertexShader;
    QOpenGLShader* surfaceGouraudShadedFragmentShader;

    QOpenGLShader* surfacePhongShadedVertexShader;
    QOpenGLShader* surfacePhongShadedFragmentShader;

    QString vertexShaderSource ;
    QString fragmentShaderSource;
    QString geometryShaderSource;

    QVector<QVector3D> controlPoints = {{100, 100,0}, {200, 50,0}, {300, 200,0}, {200,150,0}};

    QVector<QVector<QVector3D>> translatedSurfaceControlPoints;
    QVector<QVector<QVector3D>> surfaceControlPoints;

    QVector<float_t> controlPointsWeights;
    QVector<QVector<float_t>> surfaceControlPointsWeights;

    QVector3D lightDirection = { 1, -1.0, -1 };

    int selectedControlPointI = 0;
    int selectedControlPointJ = 0;

    int surfaceDensity = 10;

    int grabbedPointIndex = -1;
    int grabbedPointIndexJ = -1;

    bool isUsingLeap = false;

    bool flatShading = true;
    bool gouraudShading = false;
    bool phongShading = false;

    bool modelVisible = false;

    QFile* vertShaderFile;
    QFile* fragShaderFile;
    bool newControlPoint = false;
    bool controlPointMoved = false;

    QFile* geometryShaderFile;
    float subdivisionCurveWeight = 3/4.0;

    bool clearColor = true;

    void repaintAtMoving();

public Q_SLOTS:
    void updateVertexShaderSource(const QString& source);
    void updateFragmentShaderSource(const QString& source);
    void updateGeometryShaderSource(const QString& source);

    void loadModel(const QString& filePath);

    void setFlatShading(bool shading);
    void setGouraudShading(bool shading);
    void setPhongShading(bool shading);

    void setSelectedControlPointI(int value);
    void setSelectedControlPointJ(int value);

    void setSelectedControlPointX(int value);
    void setSelectedControlPointY(int value);
    void setSelectedControlPointZ(int value);


    void setHandDirection(  const Leap::Vector& vec);
    void setHandVelocity  ( const Leap::Vector& vec);

    void setUsingLeap(bool isusing);

    void setBSplineSurfaceVisibility(bool visible);
    void setBSplineSurfaceControlNetVisibility(bool visible);

    void setBSplineVisibility(bool visible);
    void setBezierVisibility(bool visible);
    void setRationalBezierVisibility(bool visible);
    void setNurbsVisibility(bool visible);
    void setBSplineShadedVisibility(bool visible);
    void setSubdivisionCurveVisibility(bool visible);

    void setMoving(bool moving);
    void setAddingControlPoints(bool adding);

    void resetWeights();

    void setBackGroundColor(const QColor& color);

    void setBezierCurveColor(const QColor& color);
    void setBSplineSurfaceColor(const QColor& color);
    void setBSplineSurfaceCnetColor(const QColor& color);
    void setBSplineSurfaceCpointsColor(const QColor& color);

    void rotationXchanged(int rotation);
    void rotationZchanged(int rotation);

    void setBSplineCurveColor(const QColor& color);
    void setNurbsColor(const QColor& color);
    void setRationalBezierColor(const QColor& color);
    void setControlPointsColor(const QColor& color);
    void setSubdivisionCurveIterationsCount(int value);

    void recalculateSurfaceWithDensity(int density);

    void emitSelectedControlpointSliderValues(int changed);

    void setModelVisibility(bool visible);


private Q_SLOTS:

    void emitLcdNumberValue(int number);
    void emitAddingDisabled(int controlPointsCount);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* e) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* e) Q_DECL_OVERRIDE;

Q_SIGNALS:

    void setVisibilityOfBezierCurve(bool visible);
    void setVisibilityOfBSplineCurve(bool visible);
    void setVisibilityOfRationalBezierCurve(bool visible);
    void setVisibilityOfNurbsCurve(bool visible);
    void setVisibilityOfBSplineSurface(bool visible);
    void setVisibilityOfBSplineSurfaceControlNet(bool visible);
    void setVisibilityOfBSplineSurfaceShaded(bool visible);
    void setVisibilityOfSubdivisionCurve(bool visible);

    void setUseLeap(bool use);
    void setLcdNumberValue(int value);
    void setAddingControlPointsDisabled(bool disabled);
    void setResetButtonEnabled(bool enabled);
    void setSelectedControlpointSliderValues(float x, float y, float z);
};

#endif // CANVAS_H
