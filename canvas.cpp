#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QOpenGLWidget( parent),
    radiobuttonMode(0)
{
    resize(640,510);

    Q_INIT_RESOURCE(myresources);
    listener = new LeapListener();
    controller.addListener(*this->listener);
    qRegisterMetaType<Leap::Vector>("Leap::Vector");

    bezierCurve = new BezierCurve;
    bSplineCurve =  new BSplineCurve;
    subdivisionCurve = new SubdivisionCurve;
    //suzanne = new SceneObject(":/models/models/suzanne.obj");
    // suzanne = new SceneObject(":/models/models/teapot.obj");

    // suzanne = new SceneObject(":/models/models/AK-47.obj");
    // suzanne = new SceneObject(":/models/models/Butterfly1.obj");
    // suzanne = new SceneObject(":/models/models/stalker_cs_rukzak_3.obj");
    //suzanne = new SceneObject(":/models/models/Porsche_911_GT2.obj");
/*:/models/models/cs_assault.obj
 * */
    suzanne = new SceneObject(":/models/models/icosa.obj");

    subdivisionCurve->calculate(controlPoints, 3/4.0);

    surfaceControlPoints.reserve(4);
    surfaceControlPoints.resize(4);
    int ii= 0;
    for (float i = -0.5; i <  0.5; i+= 0.25, ii++)
    {
        int jj = 0;
        surfaceControlPoints[ii].reserve(4);
        surfaceControlPoints[ii].resize(4);
        for (float j = -0.5; j <  0.5; j+= 0.25, jj++)
        {
            surfaceControlPoints[ii][jj] = QVector3D(j, i, 0);
        }
    }

    for (int i =0; i < surfaceControlPoints.size(); ++i)
    {
        QVector<float_t> tmp;
        for (int j = 0; j < surfaceControlPoints.size(); ++j)
        {
            tmp.push_back(1.0);
        }
        surfaceControlPointsWeights.push_back(tmp);
    }

    surfaceControlPoints[1][2].setZ(-3);
    surfaceControlPointsWeights[1][2] = 1.2;

    bSplineSurface = new BSplineSurface(this);
    bSplineSurface->init();
    bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, surfaceDensity);

    lightDirection.normalize();
    mModelMatrix = bSplineSurface->getTransformationMatrix();

    translatedSurfaceControlPoints = bSplineSurface->translateSurfaceControlPoints(surfaceControlPoints);

    vertShaderFile = new QFile("vertexshader.vsh");
    fragShaderFile = new QFile("fragmentshader.fsh");
    geometryShaderFile = new QFile("geometryshader.gsh");

    geometryShaderFile->open(QIODevice::ReadWrite);
    vertShaderFile->open(QIODevice::ReadWrite);
    fragShaderFile->open(QIODevice::ReadWrite);

    controlPointsWeights.reserve(controlPoints.size());
    controlPointsWeights.resize(controlPoints.size());

    emitLcdNumberValue(controlPoints.size());

    std::for_each(controlPointsWeights.begin(), controlPointsWeights.end(), [](float_t& value){value = 1.0;});
    if (vertShaderFile->isOpen())
    {
        QTextStream in(vertShaderFile);
        vertexShaderSource = in.readAll();
    }
    else
    {
        qDebug() << "vertexshader file not opened!" << "\n" << vertShaderFile->errorString();
    }

    if (fragShaderFile->isOpen())
    {
        QTextStream in(fragShaderFile);
        fragmentShaderSource = in.readAll();
    }
    else
    {
        qDebug() << "fragshader file not opened!" << "\n"<< fragShaderFile->errorString();
    }

    if (geometryShaderFile->isOpen())
    {
        QTextStream in(geometryShaderFile);
        geometryShaderSource = in.readAll();
    }
    else
    {
        qDebug() << "geometryshader file not opened!" << "\n"<< geometryShaderFile->errorString();
    }

    connect(this, &Canvas::setVisibilityOfBezierCurve,  bezierCurve,   &BezierCurve::setVisible);
    connect(this, &Canvas::setVisibilityOfBSplineCurve, bSplineCurve,  &BSplineCurve::setVisible);
    connect(this, &Canvas::setVisibilityOfNurbsCurve, bSplineCurve, &BSplineCurve::setNurbsVisible);
    connect(this, &Canvas::setVisibilityOfRationalBezierCurve, bezierCurve, &BezierCurve::setRationalVisible);
    connect(this, &Canvas::setVisibilityOfBSplineSurface, bSplineSurface, &BSplineSurface::setVisible);
    connect(this, &Canvas::setVisibilityOfBSplineSurfaceControlNet, bSplineSurface, &BSplineSurface::setControlNetVisible);
    connect(this, &Canvas::setVisibilityOfBSplineSurfaceShaded, bSplineSurface, &BSplineSurface::setShadedVisible);
    connect(this, &Canvas::setVisibilityOfSubdivisionCurve, subdivisionCurve, &SubdivisionCurve::setVisible);

    connect(listener->handler, &LeapHandler::emithandDirection, this, &Canvas::setHandDirection);
    connect(listener->handler, &LeapHandler::emithandVelocity, this, &Canvas::setHandVelocity);

    connect(listener->handler, &LeapHandler::emitIsConnected, this, &Canvas::setUsingLeap);

}

Canvas::~Canvas()
{
}
void Canvas::emitLcdNumberValue(int number)
{
    emit setLcdNumberValue(number);
}

SubdivisionCurve* Canvas::getSubdivisionCurve() const
{
    return this->subdivisionCurve;
}

void Canvas::setHandVelocity(const Leap::Vector &vec)
{
    handVelocity = vec;
    handVelocity = handVelocity.normalized();

    if (isUsingLeap && radiobuttonMode )
    {
        if (grabbedPointIndex != -1 && !bSplineSurface->getIsVisible())
        {
            controlPoints[grabbedPointIndex].setX(controlPoints[grabbedPointIndex].x() + handVelocity.x);
            controlPoints[grabbedPointIndex].setY(controlPoints[grabbedPointIndex].y() + handVelocity.y );
        }

        if ( (grabbedPointIndex != -1) && (grabbedPointIndexJ != -1) && bSplineSurface->getIsVisible() )
        {

        }
    }

    if (isUsingLeap && modelVisible)
    {
        rotationXchanged( bSplineSurface->getXrotation() + handVelocity.x );
        rotationZchanged( bSplineSurface->getZrotation() + handVelocity.z );
    }

    repaint();
}

void Canvas::recalculateSurfaceWithDensity(int density)
{
    bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, density);
    surfaceDensity = density;

    repaint();
}

void Canvas::emitSelectedControlpointSliderValues(int changed)
{
    emit setSelectedControlpointSliderValues(surfaceControlPoints[selectedControlPointI][selectedControlPointJ].x(),
                                             surfaceControlPoints[selectedControlPointI][selectedControlPointJ].y(),
                                             surfaceControlPoints[selectedControlPointI][selectedControlPointJ].z());
}

void Canvas::setSelectedControlPointI(int value)
{
    selectedControlPointI = value;
}

void Canvas::setSelectedControlPointX(int value)
{
    surfaceControlPoints[selectedControlPointI][selectedControlPointJ].setX(value / 100.0);

    bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, bSplineSurface->getDensity());
    recalculateSurface();

    repaint();
}

void Canvas::setSelectedControlPointY(int value)
{
    surfaceControlPoints[selectedControlPointI][selectedControlPointJ].setY(value / 100.0);

    bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, bSplineSurface->getDensity());

    recalculateSurface();
    repaint();
}

void Canvas::setSelectedControlPointZ(int value)
{
    surfaceControlPoints[selectedControlPointI][selectedControlPointJ].setZ(value / 100.0);

    bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, bSplineSurface->getDensity());

    recalculateSurface();
    repaint();
}

void Canvas::recalculateSurface()
{
    mBSplineSurfaceCnetVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.allocate(surfaceControlPoints.size() *4 * 4* sizeof(QVector3D));
    int offset = 0;
    for (int i = 0; i < surfaceControlPoints.size(); ++i)
    {
        for (int j = 0; j < surfaceControlPoints[i].size(); ++j)
        {
            mVertexBuffer.write(offset++*  sizeof(QVector3D),
                                &surfaceControlPoints[i][j],   sizeof(QVector3D));
        }
    }

    for (int i = 0; i < surfaceControlPoints.size(); ++i)
    {
        for (int j = 0; j < surfaceControlPoints[i].size(); ++j)
        {
            mVertexBuffer.write( offset++*sizeof(QVector3D), &surfaceControlPoints[j][i], sizeof(QVector3D));
        }
    }
    mSurfaceNetProgram->enableAttributeArray(0);
    mSurfaceNetProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    mBSplineSurfaceCnetVAO.release();
    mVertexBuffer.destroy();

    //

    mBSplineSurfaceShadedVAO.bind();

    mNormalBuffer.create();
    mNormalBuffer.bind();
    mNormalBuffer.allocate(( bSplineSurface->getTriangleNormalsCount() + bSplineSurface->trianglesCount()) * sizeof(QVector3D));

    mNormalBuffer.write(0, bSplineSurface->getTrianglePoints(), bSplineSurface->trianglesCount() * sizeof(QVector3D));

    mShaderProgram->enableAttributeArray(0);
    mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    mNormalBuffer.write(bSplineSurface->trianglesCount() * sizeof(QVector3D),
                        bSplineSurface->getTriangleFaceNormals()  ,
                        bSplineSurface->getTriangleNormalsCount() * sizeof(QVector3D));

    mShaderProgram->enableAttributeArray(1);
    mShaderProgram->setAttributeBuffer(1, GL_FLOAT, bSplineSurface->trianglesCount() * sizeof(QVector3D), 3, 0);

    mBSplineSurfaceShadedVAO.release();
    mNormalBuffer.destroy();

    //surface vertical lines VAO
    mBSplineSurfaceVerticalLinesVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate(bSplineSurface->getVerticalBsplinesCount() * bSplineSurface->getSurfaceVerPoints(0).size() * sizeof(QVector3D));

    for (int i = 0; i < bSplineSurface->getVerticalBsplinesCount(); ++i)
    {
        mVertexBuffer.write(i * bSplineSurface->getSurfaceVerPoints(i).size() * sizeof(QVector3D),
                            bSplineSurface->getSurfaceVerPoints(i).constData(), bSplineSurface->getSurfaceVerPoints(i).size() * sizeof(QVector3D));
    }

    mShaderProgram->enableAttributeArray(0);
    mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    mBSplineSurfaceVerticalLinesVAO.release();
    mVertexBuffer.destroy();

    //surface horizontal lines VAO
    mBSplineSurfaceHorizontalLinesVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate(bSplineSurface->getHorizontalBsplinesCount() * bSplineSurface->getSurfaceHorPoints(0).size() * sizeof(QVector3D));

    for (int i = 0; i < bSplineSurface->getHorizontalBsplinesCount(); ++i)
    {
        mVertexBuffer.write(i * bSplineSurface->getSurfaceHorPoints(i).size() * sizeof(QVector3D),
                            bSplineSurface->getSurfaceHorPoints(i).constData(), bSplineSurface->getSurfaceHorPoints(i).size() * sizeof(QVector3D));
    }

    mShaderProgram->enableAttributeArray(0);
    mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    mBSplineSurfaceHorizontalLinesVAO.release();
    mVertexBuffer.destroy();
}

void Canvas::setSelectedControlPointJ(int value)
{
    selectedControlPointJ = value;
}

void Canvas::wheelEvent(QWheelEvent *e)
{
    if (radiobuttonMode && (grabbedPointIndex != -1) && grabbedPointIndexJ == -1)
    {
        if (e->angleDelta().y() > 0)
        {
            controlPointsWeights[grabbedPointIndex] *= 1.1;

            emit setResetButtonEnabled(true);
        }
        else
        {
            controlPointsWeights[grabbedPointIndex] *= 0.9;
            emit setResetButtonEnabled(true);
        }
    }

    if (radiobuttonMode && bSplineSurface->getIsVisible())
    {
        if (grabbedPointIndexJ != -1 && grabbedPointIndex != -1)
        {
            if (e->angleDelta().y() > 0)
            {
                surfaceControlPointsWeights[grabbedPointIndex][grabbedPointIndexJ] *= 1.1;
                emit setResetButtonEnabled(true);
            }
            else
            {
                surfaceControlPointsWeights[grabbedPointIndex][grabbedPointIndexJ] *= 0.9;
                emit setResetButtonEnabled(true);
            }
        }
    }

    bezierCurve->calculateRationalPoints(controlPoints, controlPointsWeights);
    bSplineCurve->calculateNURPoints(controlPoints, controlPointsWeights);
    bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, surfaceDensity);
    repaint();
}

QOpenGLShader* Canvas::instantiateShader(QOpenGLShader *shader, const QString &filename, QOpenGLShader::ShaderTypeBit type)
{
    shader = new QOpenGLShader(type);
    shader->compileSourceFile(filename);
    if (shader->isCompiled())
    {
        qDebug() << filename << " compiled!";
    }
    return shader;
}

void Canvas::reinitialiseModel()
{
    //suzanne

    suzanne->destroyVao();
    if (! suzanne->createVao())
    {
        qDebug() << "Failed to create vao";
    }
    else
    {
        qDebug() << "vao created!";

        suzanne->bindVao();

        mNormalBuffer.create();
        mNormalBuffer.bind();
        mNormalBuffer.allocate(( suzanne->getVerticesCount() + suzanne->getNormalsCount()) * sizeof(QVector3D));

        mShaderProgram->enableAttributeArray(0);
        mNormalBuffer.write(0, suzanne->getVertices(), suzanne->getVerticesCount() * sizeof(QVector3D));
        mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
        mShaderProgram->disableAttributeArray(0);

        mShaderProgram->enableAttributeArray(1);
        mNormalBuffer.write(suzanne->getVerticesCount() * sizeof(QVector3D),
                            suzanne->getNormals()  ,
                            suzanne->getNormalsCount() * sizeof(QVector3D));
        mShaderProgram->setAttributeBuffer(1, GL_FLOAT, suzanne->getVerticesCount() * sizeof(QVector3D), 3, 0);
        mShaderProgram->disableAttributeArray(1);

        suzanne->releaseVao();
        mNormalBuffer.destroy();

    }

    /*mIndexBuffer.create();
    mIndexBuffer.bind();
    mIndexBuffer.allocate(suzanne->getVerticesIndicesSize() * sizeof(QVector3D));
    mIndexBuffer.write(0, &suzanne->getVerticesIndices(), suzanne->getVerticesIndicesSize() * sizeof(QVector3D));
    mIndexBuffer.release();
*/
}

void Canvas::initializeGL()
{
    mVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    mNormalBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    mIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    //mVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    // mNormalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    curvesVertexShader = instantiateShader(curvesVertexShader,     QString(":/shaders/vertexshader.vsh"), QOpenGLShader::Vertex);
    curvesFragmentShader = instantiateShader(curvesFragmentShader, QString(":/shaders/fragmentshader.fsh"), QOpenGLShader::Fragment);
    curvesGeometryShader = instantiateShader(curvesGeometryShader, QString(":/shaders/geometryshader.gsh"), QOpenGLShader::Geometry);

    surfaceFlatShadedVertexShader = instantiateShader(surfaceFlatShadedVertexShader,      QString(":/shaders/flatshadedsurfacevertexshader.vert"), QOpenGLShader::Vertex);
    surfaceFlatShadedFragmentShader = instantiateShader(surfaceFlatShadedFragmentShader, QString(":/shaders/flatshadedsurfacefragmentshader.frag"), QOpenGLShader::Fragment);

    surfacePhongShadedVertexShader = instantiateShader(surfacePhongShadedVertexShader,    QString(":/shaders/phongshadedsurfacevertexshader.vert"), QOpenGLShader::Vertex);
    surfacePhongShadedFragmentShader = instantiateShader(surfacePhongShadedFragmentShader, QString(":/shaders/phongshadedsurfacefragmentshader.frag"), QOpenGLShader::Fragment);

    surfaceGouraudShadedVertexShader = instantiateShader(surfaceGouraudShadedVertexShader,    QString(":/shaders/gouraudshadedsurfacevertexshader.vert"), QOpenGLShader::Vertex);
    surfaceGouraudShadedFragmentShader = instantiateShader(surfaceGouraudShadedFragmentShader, QString(":/shaders/gouraudshadedsurfacefragmentshader.frag"), QOpenGLShader::Fragment);

    surfaceNetFragmentShader = instantiateShader(surfaceNetFragmentShader, QString(":/shaders/surfacenetfragmentshader.frag"), QOpenGLShader::Fragment);

    computeShader = instantiateShader(computeShader, QString(":/shaders/computeshader.cs"), QOpenGLShader::Compute);

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    mCurvesProgram = new QOpenGLShaderProgram();
    mShaderProgram = new QOpenGLShaderProgram();
    mComputeShaderProgram = new QOpenGLShaderProgram();
    mGouraudShaderProgram = new QOpenGLShaderProgram();
    mFlatShaderProgram = new QOpenGLShaderProgram();
    mSurfaceNetProgram = new QOpenGLShaderProgram();

    mComputeShaderProgram->addShader(computeShader);

    mCurvesProgram->addShader(curvesVertexShader);
    mCurvesProgram->addShader(curvesFragmentShader);

    /* mShaderProgram->addShader(surfaceFlatShadedVertexShader);
    mShaderProgram->addShader(surfaceFlatShadedFragmentShader);
    mSurfaceNetProgram->addShader(surfaceFlatShadedVertexShader);*/


    mShaderProgram->addShader(surfaceGouraudShadedVertexShader);
    mShaderProgram->addShader(surfaceGouraudShadedFragmentShader);

    mSurfaceNetProgram->addShader(surfaceNetFragmentShader);
    mSurfaceNetProgram->addShader(surfaceGouraudShadedVertexShader);

    mFlatShaderProgram->addShader(surfaceFlatShadedVertexShader);
    mFlatShaderProgram->addShader(surfaceFlatShadedFragmentShader);

    mGouraudShaderProgram->addShader(surfaceGouraudShadedVertexShader);
    mGouraudShaderProgram->addShader(surfaceGouraudShadedFragmentShader);

    mSurfaceNetProgram->link();
    mCurvesProgram->link();
    mShaderProgram->link();
    mGouraudShaderProgram->link();
    mFlatShaderProgram->link();

    getUniformLocation(mShaderProgram, mModelViewProjectionMatrixLocation, "u_modelViewProjectionMatrix" );
    getUniformLocation(mShaderProgram, mControlPointsLocation, "u_controlPoints" );
    getUniformLocation(mShaderProgram, mNumControlPointsLocation, "u_numControlPoints" );
    getUniformLocation(mShaderProgram, mLightDirectionLocation, "u_lightDirection" );
    getUniformLocation(mShaderProgram, mModelMatrixLocation, "u_modelMatrix" );

    getUniformLocation(mCurvesProgram, mModelViewProjectionMatrixLocationCurvesProgram, "u_modelViewProjectionMatrix" );
    getUniformLocation(mCurvesProgram, mColorLocationCurvesProgram, "u_color" );

    getUniformLocation(mSurfaceNetProgram, mColorLocation, "u_color");
    getUniformLocation(mSurfaceNetProgram, mModelViewProjectionMatrixLocationControlNetProgram, "u_modelViewProjectionMatrix");
    getUniformLocation(mSurfaceNetProgram, mModelMatrixLocationControlNetProgram, "u_modelMatrix");

    bezierCurve->calculatePoints(controlPoints);
    bezierCurve->calculateRationalPoints(controlPoints, controlPointsWeights);
    bSplineCurve->calculatePoints(controlPoints);
    bSplineCurve->calculateNURPoints(controlPoints, controlPointsWeights);

    mControlPointsVAO.create();
    mBezierCurveVAO.create();
    mBSplineCurveVAO.create();
    mRationalBezierCurveVAO.create();
    mNurbsCurveVAO.create();
    mBSplineSurfaceCpVerticalVAO.create();
    mBSplineSurfaceCpHorizontalVAO.create();

    mBSplineSurfaceVerticalLinesVAO.create();
    mBSplineSurfaceHorizontalLinesVAO.create();
    mBSplineSurfaceShadedVAO.create();
    mBSplineSurfaceCnetVAO.create();
    mSubdivisionCurveVAO.create();

    mCurvesProgram->bind();
    mCurvesProgram->setUniformValue(mModelViewProjectionMatrixLocationCurvesProgram,  mProjectionMatrix  );

    //// curves controlpoints
    mControlPointsVAO.bind();

    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate( controlPoints.size()* sizeof(QVector3D));

    mVertexBuffer.write(0, controlPoints.constData(), controlPoints.size() * sizeof(QVector3D));
    mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
    mCurvesProgram->enableAttributeArray(0);

    mControlPointsVAO.release();
    mVertexBuffer.destroy();

    //bezier curve
    mBezierCurveVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate( bezierCurve->pointsCount()* sizeof(typeid(controlPoints)));
    mVertexBuffer.write(0, bezierCurve->getBezierPoints(), bezierCurve->pointsCount() * sizeof(QVector3D));
    mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
    mCurvesProgram->enableAttributeArray(0);

    mBezierCurveVAO.release();
    mVertexBuffer.destroy();

    //bspline curve
    mBSplineCurveVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate( bSplineCurve->pointsCount()* sizeof(typeid(controlPoints)));
    mVertexBuffer.write(0, bSplineCurve->getBSplinePoints(), bSplineCurve->pointsCount() * sizeof(QVector3D));
    mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
    mCurvesProgram->enableAttributeArray(0);

    mBSplineCurveVAO.release();
    mVertexBuffer.destroy();

    //nurbs curve
    mNurbsCurveVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate( bSplineCurve->nurbsPointsCount() * sizeof(typeid(controlPoints)));
    mVertexBuffer.write(0, bSplineCurve->getNURBSPoints(), bSplineCurve->nurbsPointsCount() * sizeof(QVector3D));
    mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
    mCurvesProgram->enableAttributeArray(0);

    mNurbsCurveVAO.release();
    mVertexBuffer.destroy();

    //rational bezier curve
    mRationalBezierCurveVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate(bezierCurve->rationalPointsCount() * sizeof(typeid(controlPoints)));
    mVertexBuffer.write(0, bezierCurve->getRationalBezierPoints(), bezierCurve->rationalPointsCount() * sizeof(QVector3D));
    mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
    mCurvesProgram->enableAttributeArray(0);

    mRationalBezierCurveVAO.release();
    mVertexBuffer.destroy();

    //subdivision curve

    mSubdivisionCurveVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.allocate(subdivisionCurve->subdivisionCurvePointsCount() * sizeof(QVector3D));
    mVertexBuffer.write(0, subdivisionCurve->getSubdivisionPoints(), subdivisionCurve->subdivisionCurvePointsCount() * sizeof(QVector3D));

    mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    mCurvesProgram->enableAttributeArray(0);

    mSubdivisionCurveVAO.release();
    mVertexBuffer.destroy();

    // surface control net VAO
    mBSplineSurfaceCnetVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.allocate(surfaceControlPoints.size() *4 * 4* sizeof(QVector3D));
    int offset = 0;
    for (int i = 0; i < surfaceControlPoints.size(); ++i)
    {
        for (int j = 0; j < surfaceControlPoints[i].size(); ++j)
        {
            mVertexBuffer.write(offset++*  sizeof(QVector3D),
                                &surfaceControlPoints[i][j],   sizeof(QVector3D));
        }
    }

    for (int i = 0; i < surfaceControlPoints.size(); ++i)
    {
        for (int j = 0; j < surfaceControlPoints[i].size(); ++j)
        {
            mVertexBuffer.write( offset++*sizeof(QVector3D), &surfaceControlPoints[j][i], sizeof(QVector3D));
        }
    }

    mSurfaceNetProgram->enableAttributeArray(0);
    mSurfaceNetProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    mBSplineSurfaceCnetVAO.release();
    mVertexBuffer.destroy();

    //surface vertical lines VAO
    mBSplineSurfaceVerticalLinesVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate(bSplineSurface->getVerticalBsplinesCount() * bSplineSurface->getSurfaceVerPoints(0).size() * sizeof(QVector3D));

    for (int i = 0; i < bSplineSurface->getVerticalBsplinesCount(); ++i)
    {
        mVertexBuffer.write(i * bSplineSurface->getSurfaceVerPoints(i).size() * sizeof(QVector3D),
                            bSplineSurface->getSurfaceVerPoints(i).constData(), bSplineSurface->getSurfaceVerPoints(i).size() * sizeof(QVector3D));
    }

    mShaderProgram->enableAttributeArray(0);
    mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    mBSplineSurfaceVerticalLinesVAO.release();
    mVertexBuffer.destroy();

    //surface horizontal lines VAO
    mBSplineSurfaceHorizontalLinesVAO.bind();
    mVertexBuffer.create();
    mVertexBuffer.bind();

    mVertexBuffer.allocate(bSplineSurface->getHorizontalBsplinesCount() * bSplineSurface->getSurfaceHorPoints(0).size() * sizeof(QVector3D));

    for (int i = 0; i < bSplineSurface->getHorizontalBsplinesCount(); ++i)
    {
        mVertexBuffer.write(i * bSplineSurface->getSurfaceHorPoints(i).size() * sizeof(QVector3D),
                            bSplineSurface->getSurfaceHorPoints(i).constData(), bSplineSurface->getSurfaceHorPoints(i).size() * sizeof(QVector3D));
    }

    mShaderProgram->enableAttributeArray(0);
    mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    mBSplineSurfaceHorizontalLinesVAO.release();
    mVertexBuffer.destroy();

#if 1
    mBSplineSurfaceShadedVAO.bind();

    mNormalBuffer.create();
    mNormalBuffer.bind();
    mNormalBuffer.allocate(( bSplineSurface->getTriangleNormalsCount() + bSplineSurface->trianglesCount()) * sizeof(QVector3D));

    mShaderProgram->enableAttributeArray(0);
    mNormalBuffer.write(0, bSplineSurface->getTrianglePoints(), bSplineSurface->trianglesCount() * sizeof(QVector3D));
    mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    mShaderProgram->enableAttributeArray(1);
    mNormalBuffer.write(bSplineSurface->trianglesCount() * sizeof(QVector3D),
                        bSplineSurface->getTriangleFaceNormals()  ,
                        bSplineSurface->getTriangleNormalsCount() * sizeof(QVector3D));
    mShaderProgram->setAttributeBuffer(1, GL_FLOAT, bSplineSurface->trianglesCount() * sizeof(QVector3D), 3, 0);

    mBSplineSurfaceShadedVAO.release();
    mNormalBuffer.destroy();

#else
    /*  mBSplineSurfaceShadedVAO.bind();

    mNormalBuffer.create();
    mNormalBuffer.bind();
    mNormalBuffer.allocate(( bSplineSurface->getTriangleVertexNormalsCount() + bSplineSurface->trianglesCount()) * sizeof(QVector3D));

    mNormalBuffer.write(0, bSplineSurface->getTrianglePoints(), bSplineSurface->trianglesCount() * sizeof(QVector3D));
    mNormalBuffer.write(bSplineSurface->trianglesCount() * sizeof(QVector3D),
                        bSplineSurface->getTriangleVertexNormals()  ,
                        bSplineSurface->getTriangleVertexNormalsCount() * sizeof(QVector3D));

    mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    mShaderProgram->setAttributeBuffer(1, GL_FLOAT, bSplineSurface->trianglesCount() * sizeof(QVector3D), 3, 0);

    mShaderProgram->enableAttributeArray(0);
    mShaderProgram->enableAttributeArray(1);
    mBSplineSurfaceShadedVAO.release();
    mVertexBuffer.destroy();
*/

#endif
    //suzanne
    suzanne->createVao();
    suzanne->bindVao();

    mNormalBuffer.create();
    mNormalBuffer.bind();
    mNormalBuffer.allocate(( suzanne->getVerticesCount() + suzanne->getNormalsCount()) * sizeof(QVector3D));

    mShaderProgram->enableAttributeArray(0);
    mNormalBuffer.write(0, suzanne->getVertices(), suzanne->getVerticesCount() * sizeof(QVector3D));
    mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    mShaderProgram->enableAttributeArray(1);
    mNormalBuffer.write(suzanne->getVerticesCount() * sizeof(QVector3D),
                        suzanne->getNormals()  ,
                        suzanne->getNormalsCount() * sizeof(QVector3D));
    mShaderProgram->setAttributeBuffer(1, GL_FLOAT, suzanne->getVerticesCount() * sizeof(QVector3D), 3, 0);

    suzanne->releaseVao();
    mNormalBuffer.destroy();

    /*mIndexBuffer.create();
    mIndexBuffer.bind();
    mIndexBuffer.allocate(suzanne->getVerticesIndicesSize() * sizeof(QVector3D));
    mIndexBuffer.write(0, &suzanne->getVerticesIndices(), suzanne->getVerticesIndicesSize() * sizeof(QVector3D));
*/

    //    mIndexBuffer.release();

    glEnable(GL_POINT_SMOOTH);
    mShaderProgram->release();
}

void Canvas::updateVertexShaderSource(const QString &source)
{
    vertShaderFile->resize(0);
    QTextStream stream(vertShaderFile);
    stream << source;

    vertexShaderSource = source;
    qDebug() <<vertexShaderSource;
}

void Canvas::getUniformLocation(QOpenGLShaderProgram* program, int &location, const char *name)
{
    location = program->uniformLocation(name);
}

void Canvas::updateGeometryShaderSource(const QString &source)
{
    geometryShaderFile->resize(0);
    QTextStream stream(geometryShaderFile);
    stream << source;

    geometryShaderSource = source;
    qDebug() << geometryShaderSource;
}

void Canvas::updateFragmentShaderSource(const QString &source)
{
    fragShaderFile->resize(0);
    QTextStream stream(fragShaderFile);

    stream << source;
    fragmentShaderSource = source;

    qDebug() << fragmentShaderSource;
}

void Canvas::setBackGroundColor(const QColor& color)
{
    backgroundColor = color;
    clearColor = true;

    repaint();
}

void Canvas::setBezierCurveColor(const QColor &color)
{
    bezierCurveColor = color;
    repaint();
}

void Canvas::setBSplineCurveColor(const QColor &color)
{
    bsplineCurveColor = color;
    repaint();
}

void Canvas::setNurbsColor(const QColor &color)
{
    nurbsColor = color;
    repaint();
}

void Canvas::setRationalBezierColor(const QColor &color)
{
    rationalBezierCurveColor = color;
    repaint();
}

void Canvas::setControlPointsColor(const QColor &color)
{
    controlPointsColor = color;
    repaint();
}

void Canvas::setBSplineSurfaceColor(const QColor &color)
{
    bsplineSurfaceColor = color;
    repaint();
}

void Canvas::setBSplineSurfaceCpointsColor(const QColor &color)
{
    bsplineSurfaceCpointsColor = color;
    repaint();
}

void Canvas::setBSplineSurfaceCnetColor(const QColor &color)
{
    bsplineSurfaceCnetColor = color;
    repaint();
}

void Canvas::setHandDirection(const Leap::Vector &vec)
{
    handDirection = vec;
}

void Canvas::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (! bSplineSurface->getIsVisible() && !bSplineSurface->getIsShadedVisible() && !modelVisible)
    {
        mCurvesProgram->bind();
        mCurvesProgram->setUniformValue(mModelViewProjectionMatrixLocationCurvesProgram,  mProjectionMatrix  );
        glPointSize(6.5f);
        mCurvesProgram->setUniformValue(mColorLocationCurvesProgram, controlPointsColor.redF(), controlPointsColor.blueF(), bezierCurveColor.greenF());

        if (newControlPoint || controlPointMoved)
        {
            mControlPointsVAO.bind();

            mVertexBuffer.create();
            mVertexBuffer.bind();

            mVertexBuffer.allocate(controlPoints.size()* sizeof(QVector3D));

            mVertexBuffer.write(0, controlPoints.constData(), controlPoints.size() * sizeof(QVector3D));
            mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
            mCurvesProgram->enableAttributeArray(0);
            glDrawArrays(GL_POINTS, 0, controlPoints.size());
            glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());

            mControlPointsVAO.release();
            mVertexBuffer.destroy();
        }
        else
        {
            mControlPointsVAO.bind();
            glDrawArrays(GL_POINTS, 0, controlPoints.size());
            glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());
            mControlPointsVAO.release();
        }

        if (bezierCurve->getIsVisible())
        {
            mCurvesProgram->setUniformValue(mColorLocationCurvesProgram, bezierCurveColor.redF(), bezierCurveColor.blueF(), bezierCurveColor.greenF());

            if (newControlPoint || controlPointMoved)
            {
                mBezierCurveVAO.bind();
                mVertexBuffer.create();
                mVertexBuffer.bind();

                mVertexBuffer.allocate( bezierCurve->pointsCount()* sizeof(typeid(controlPoints)));
                mVertexBuffer.write(0, bezierCurve->getBezierPoints(), bezierCurve->pointsCount() * sizeof(QVector3D));
                mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
                mCurvesProgram->enableAttributeArray(0);
                glDrawArrays(GL_LINE_STRIP, 0, bezierCurve->pointsCount());

                mBezierCurveVAO.release();
                mVertexBuffer.destroy();
            }
            else
            {
                mBezierCurveVAO.bind();
                glDrawArrays(GL_LINE_STRIP, 0, bezierCurve->pointsCount());
                mBezierCurveVAO.release();
            }
        }

        if (bezierCurve->getIsRationalVisible())
        {
            mCurvesProgram->setUniformValue(mColorLocationCurvesProgram, rationalBezierCurveColor.redF(), rationalBezierCurveColor.greenF(), rationalBezierCurveColor.blueF());

            if (newControlPoint || controlPointMoved)
            {
                mRationalBezierCurveVAO.bind();
                mVertexBuffer.create();
                mVertexBuffer.bind();

                mVertexBuffer.allocate(bezierCurve->rationalPointsCount() * sizeof(typeid(controlPoints)));
                mVertexBuffer.write(0, bezierCurve->getRationalBezierPoints(), bezierCurve->rationalPointsCount() * sizeof(QVector3D));
                mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
                mCurvesProgram->enableAttributeArray(0);
                glDrawArrays(GL_LINE_STRIP, 0, bezierCurve->rationalPointsCount());

                mRationalBezierCurveVAO.release();
                mVertexBuffer.destroy();
            }
            else
            {
                mRationalBezierCurveVAO.bind();
                glDrawArrays(GL_LINE_STRIP, 0, bezierCurve->rationalPointsCount());
                mRationalBezierCurveVAO.release();
            }

        }

        if (bSplineCurve->getIsVisible())
        {
            mCurvesProgram->setUniformValue(mColorLocationCurvesProgram, bsplineCurveColor.redF(), bsplineCurveColor.greenF(), bsplineCurveColor.blueF());
            if (newControlPoint || controlPointMoved)
            {
                mBSplineCurveVAO.bind();
                mVertexBuffer.create();
                mVertexBuffer.bind();

                mVertexBuffer.allocate( bSplineCurve->pointsCount()* sizeof(typeid(controlPoints)));
                mVertexBuffer.write(0, bSplineCurve->getBSplinePoints(), bSplineCurve->pointsCount() * sizeof(QVector3D));
                mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3,0);
                mCurvesProgram->enableAttributeArray(0);
                glDrawArrays(GL_LINE_STRIP, 0, bSplineCurve->pointsCount());

                mBSplineCurveVAO.release();
                mVertexBuffer.destroy();
            }
            else
            {
                mBSplineCurveVAO.bind();
                glDrawArrays(GL_LINE_STRIP, 0, bSplineCurve->pointsCount());
                mBSplineCurveVAO.release();
            }

        }

        if (bSplineCurve->getIsNurbsVisible())
        {
            mCurvesProgram->setUniformValue(mColorLocationCurvesProgram, nurbsColor.redF(), nurbsColor.greenF(), nurbsColor.blueF());

            mNurbsCurveVAO.bind();
            mVertexBuffer.create();
            mVertexBuffer.bind();
            mVertexBuffer.allocate(bSplineCurve->getNURBSPoints(), bSplineCurve->nurbsPointsCount() * sizeof(QVector3D));

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

            glDrawArrays(GL_LINE_STRIP, 0, bSplineCurve->nurbsPointsCount());

            mShaderProgram->disableAttributeArray(0);
            mVertexBuffer.destroy();
            glDisableVertexAttribArray(0);

            mNurbsCurveVAO.release();
        }

        if (subdivisionCurve->getIsVisible())
        {
            mCurvesProgram->setUniformValue(mColorLocationCurvesProgram,1.0, 0.0, .0);

            if (newControlPoint || controlPointMoved)
            {
                mSubdivisionCurveVAO.bind();
                mVertexBuffer.create();
                mVertexBuffer.bind();
                mVertexBuffer.allocate(subdivisionCurve->subdivisionCurvePointsCount() * sizeof(QVector3D));
                mVertexBuffer.write(0, subdivisionCurve->getSubdivisionPoints(), subdivisionCurve->subdivisionCurvePointsCount() * sizeof(QVector3D));

                mCurvesProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
                mCurvesProgram->enableAttributeArray(0);
                glDrawArrays(GL_LINE_LOOP, 0, subdivisionCurve->subdivisionCurvePointsCount());

                mSubdivisionCurveVAO.release();
                mVertexBuffer.destroy();
            }
            else
            {
                mSubdivisionCurveVAO.bind();
                glDrawArrays(GL_LINE_LOOP, 0, subdivisionCurve->subdivisionCurvePointsCount());
                mSubdivisionCurveVAO.release();
            }

        }

        if (newControlPoint || controlPointMoved)
        {
            newControlPoint = false;
            controlPointMoved = false;
        }
    }
    else
    {
        mShaderProgram->bind();
        mShaderProgram->setUniformValue(mModelViewProjectionMatrixLocation,  mProjectionMatrix * mModelMatrix );
        mShaderProgram->setUniformValue(mModelMatrixLocation, mModelMatrix);
        mShaderProgram->setUniformValue(mLightDirectionLocation, lightDirection);
        if (bSplineSurface->getIsShadedVisible())
        {
            mBSplineSurfaceShadedVAO.bind();
            glDrawArrays(GL_TRIANGLES, 0, bSplineSurface->trianglesCount());
            mBSplineSurfaceShadedVAO.release();

        }

        if (modelVisible)
        {
            suzanne->bindVao();
            glDrawArrays(GL_TRIANGLES, 0, suzanne->getVerticesCount());
            suzanne->releaseVao();
        }

        if (bSplineSurface->getIsControlPointsVisible())
        {
            mSurfaceNetProgram->bind();
            mSurfaceNetProgram->setUniformValue(mColorLocation, bsplineSurfaceCnetColor.redF(), bsplineSurfaceCnetColor.greenF(), bsplineSurfaceCnetColor.blueF());
            mSurfaceNetProgram->setUniformValue(mModelViewProjectionMatrixLocationControlNetProgram,  mProjectionMatrix * mModelMatrix );
            mSurfaceNetProgram->setUniformValue(mModelMatrixLocationControlNetProgram, mModelMatrix);
            mBSplineSurfaceCnetVAO.bind();

            for (int i = 0; i < surfaceControlPoints.size(); ++i)
            {
                glDrawArrays(GL_LINE_STRIP, 4* i, surfaceControlPoints.size());
            }

            glDrawArrays(GL_POINTS, 0, surfaceControlPoints.size() * 4  );

            for (int i = 0; i < surfaceControlPoints.size(); ++i)
            {
                glDrawArrays(GL_LINE_STRIP,  4* i + surfaceControlPoints.size() * 4,  surfaceControlPoints.size() );
            }
            mBSplineSurfaceCnetVAO.release();
            mShaderProgram->bind();
        }
        if (bSplineSurface->getIsVisible())
        {
            mBSplineSurfaceVerticalLinesVAO.bind();
            for (int i = 0; i < bSplineSurface->getVerticalBsplinesCount(); ++i)
            {
                glDrawArrays(GL_LINE_STRIP, i * bSplineSurface->getSurfaceVerPoints(i).size(),
                             bSplineSurface->getSurfaceVerPoints(i).size());
            }
            mBSplineSurfaceVerticalLinesVAO.release();

            mBSplineSurfaceHorizontalLinesVAO.bind();
            for (int i = 0; i < bSplineSurface->getHorizontalBsplinesCount(); ++i)
            {
                glDrawArrays(GL_LINE_STRIP, i * bSplineSurface->getSurfaceHorPoints(i).size(),
                             bSplineSurface->getSurfaceHorPoints(i).size());
            }
            mBSplineSurfaceHorizontalLinesVAO.release();
        }
    }

    if (clearColor)
    {
        glClearColor(backgroundColor.redF(), backgroundColor.greenF() , backgroundColor.blueF(), 0);
        clearColor = false;
        repaint();
    }
    mShaderProgram->release();
}

void Canvas::setSubdivisionCurveIterationsCount(int value)
{
    subdivisionCurve->setIterations(value);

    subdivisionCurve->calculate(controlPoints, 1/1.9);

    repaint();
}

void Canvas::resizeGL(int w, int h)
{
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.ortho(0, w, h, 0, 155, -155 );
}

void Canvas::setFlatShading(bool shading)
{
    flatShading = shading;
}

void Canvas::setGouraudShading(bool shading)
{
    gouraudShading = shading;
}

void Canvas::setPhongShading(bool shading)
{
    phongShading = shading;
}

void Canvas::rotationXchanged(int rotation)
{
    bSplineSurface->updateRotation(rotation, bSplineSurface->getZrotation());
    mModelMatrix = bSplineSurface->getTransformationMatrix();

    repaint();
}

void Canvas::rotationZchanged(int rotation)
{
    bSplineSurface->updateRotation(bSplineSurface->getXrotation(), rotation);
    mModelMatrix = bSplineSurface->getTransformationMatrix();

    repaint();
}

int Canvas::getControlPointsCount() const
{
    return controlPoints.size();
}

void Canvas::mousePressEvent(QMouseEvent *e)
{
    if (this->radiobuttonMode == 0)
    {
        if( controlPoints.size() < 30)
        {
            QVector3D point{e->pos().x(),e->pos().y(),0};
            qDebug() << controlPoints.size();
            controlPoints.push_back(point);
            newControlPoint = true;

            controlPointsWeights.push_back(1.0);
            emitLcdNumberValue(controlPoints.size());

            bezierCurve->calculatePoints(controlPoints);
            bezierCurve->calculateRationalPoints(controlPoints, controlPointsWeights);
            bSplineCurve->calculatePoints(controlPoints);
            subdivisionCurve->calculate(controlPoints,subdivisionCurveWeight);

            repaint();
        }
        else
        {
            emitAddingDisabled(controlPoints.size());
        }
    }
    else if (this->radiobuttonMode == 1)
    {
        QVector3D pointGrabbed;
        for (int i = 0; i < controlPoints.size(); ++i)
        {
            pointGrabbed.setX( e->pos().x());
            pointGrabbed.setY(e->pos().y());
            pointGrabbed.setZ(0);
            if (controlPoints[i].distanceToPoint(pointGrabbed) < 5)
            {
                grabbedPointIndex = i;
                break;
            }
        }

        if (bSplineSurface->getIsVisible())
        {
            for (int i = 0; i < translatedSurfaceControlPoints.size(); ++i)
            {
                for (int j = 0; j < translatedSurfaceControlPoints.at(i).size(); ++j)
                {
                    pointGrabbed.setX( e->pos().x());
                    pointGrabbed.setY(e->pos().y());
                    pointGrabbed.setZ(0);

                    if (translatedSurfaceControlPoints[i][j].distanceToPoint(pointGrabbed) < 5)
                    {
                        qDebug() << i << ", " << j;
                        grabbedPointIndex = i;
                        grabbedPointIndexJ = j;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        QOpenGLWidget::mousePressEvent(e);
    }
}

void Canvas::resetWeights()
{
    std::for_each(controlPointsWeights.begin(), controlPointsWeights.end(), [](float& value){ value = 1.0;});

    for (auto& i: surfaceControlPointsWeights)
    {
        std::for_each(i.begin(), i.end(), [](float& value){ value = 1.0;});
    }
    bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, surfaceDensity);

    repaint();
    emit setResetButtonEnabled(false);
}

void Canvas::mouseMoveEvent(QMouseEvent *e)
{
    if (radiobuttonMode == 1)
    {
        if (grabbedPointIndex != -1 && ! bSplineSurface->getIsVisible())
        {
            if ((e->pos().x() > (x() - pos().x())) &&
                    (e->pos().x() < width()) &&
                    (e->pos().y() < height())&&
                    (e->pos().y() > (y() - pos().y())))
            {
                controlPoints[grabbedPointIndex] = QVector3D(e->pos());
                controlPointMoved = true;
                repaintAtMoving();
            }
            else if ((e->pos().x() < pos().x()) &&
                     (e->pos().y() > pos().y()) && (e->pos().y() < (pos().y() + height())))
            {
                controlPoints[grabbedPointIndex] = QVector3D(pos().x(), e->pos().y(), 0);
                controlPointMoved = true;
                repaintAtMoving();
            }
            else if ((e->pos().x() > (pos().x() + width())) &&
                     (e->pos().y() > pos().y()) && (e->pos().y() < (pos().y() + height())))
            {
                controlPoints[grabbedPointIndex] = QVector3D(pos().x() + width(), e->pos().y(), 0);
                controlPointMoved = true;
                repaintAtMoving();
            }
            else if ((e->pos().y() < pos().y()) &&
                     (e->pos().x() > pos().x()) && (e->pos().x() < (pos().x() + width())))
            {
                controlPoints[grabbedPointIndex] = QVector3D(e->pos().x(), pos().y(),0);
                controlPointMoved = true;
                repaintAtMoving();

            }
            else if ((e->pos().y() > (pos().y() + height())) &&
                     (e->pos().x() > pos().x()) && (e->pos().x() < (pos().x() + width()))   )
            {
                controlPoints[grabbedPointIndex] = QVector3D(e->pos().x(), pos().y() + height(),0);
                controlPointMoved = true;
                repaintAtMoving();
            }
            else if ((e->pos().x() < pos().x()) && (e->pos().y() < e->pos().y()) )
            {
                controlPoints[grabbedPointIndex] = QVector3D(pos().x(), pos().y(), 0);
                controlPointMoved = true;
                repaintAtMoving();
            }
            else if ( (e->pos().x() > (pos().x() + width())) && (e->pos().y() < pos().y()))
            {
                controlPoints[grabbedPointIndex] = QVector3D(pos().x()+ width(), pos().y(), 0);
                controlPointMoved = true;
                repaintAtMoving();
            }
            else if ((e->pos().x() > (pos().x()+ width())) && (e->pos().y() > (pos().y()+ height())))
            {
                controlPoints[grabbedPointIndex] = QVector3D(pos().x() + width(), pos().y() + height(),0 );
                controlPointMoved = true;
                repaintAtMoving();
            }
            else if ((e->pos().x() < pos().x() ) && (e->pos().y() > (pos().y() + height())))
            {
                controlPoints[grabbedPointIndex] = QVector3D(pos().x(), pos().y() + height(), 0);
                controlPointMoved = true;
                repaintAtMoving();
            }
        }
        if ((grabbedPointIndexJ != -1) &&( grabbedPointIndex != -1))
        {
            surfaceControlPoints[grabbedPointIndex][grabbedPointIndexJ].setX(e->pos().x() - 100 );
            surfaceControlPoints[grabbedPointIndex][grabbedPointIndexJ].setY(e->pos().y() - 50);

            translatedSurfaceControlPoints.clear();
            translatedSurfaceControlPoints = bSplineSurface->translateSurfaceControlPoints(surfaceControlPoints);

            bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, surfaceDensity);
            repaint();
        }
    }
    else
    {
        QOpenGLWidget::mouseMoveEvent(e);
    }
}

void Canvas::repaintAtMoving()
{
    if (bezierCurve->getIsVisible())
    {
        bezierCurve->calculatePoints(controlPoints);
    }
    if (bezierCurve->getIsRationalVisible())
    {
        bezierCurve->calculateRationalPoints(controlPoints, controlPointsWeights);
    }
    if (bSplineCurve->getIsVisible())
    {
        bSplineCurve->calculatePoints(controlPoints);
    }
    if (bSplineCurve->getIsNurbsVisible())
    {
        bSplineCurve->calculateNURPoints(controlPoints, controlPointsWeights);
    }
    if (subdivisionCurve->getIsVisible())
    {
        subdivisionCurve->calculate(controlPoints, subdivisionCurveWeight);
    }
    repaint();
}

void Canvas::setUsingLeap(bool isusing)
{
    if (listener->isConnected())
    {
        qDebug()  << "Leap connected!";
        emit setUseLeap(true);
        isUsingLeap = isusing;
    }
    else
    {
        qDebug()  << "Leap not connected!";

        emit setUseLeap(false);
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *e)
{
    if (radiobuttonMode == 1)
    {
        if (!isUsingLeap)
        {
            grabbedPointIndex = -1;
            grabbedPointIndexJ = -1;
        }
    }
    else
    {
        QOpenGLWidget::mouseReleaseEvent(e);
    }
}

void Canvas::emitAddingDisabled(int controlPointsCount)
{
    if (controlPointsCount == 30)
    {
        emit setAddingControlPointsDisabled(false);
    }
}

QString& Canvas::getFragmentShaderSource()
{
    return fragmentShaderSource;
}

QString& Canvas::getVertexShaderSource()
{
    return vertexShaderSource;
}

QString& Canvas::getGeometryShaderSource()
{
    return geometryShaderSource;
}

void Canvas::setSubdivisionCurveVisibility(bool visible)
{
    emit setVisibilityOfSubdivisionCurve(visible);
    repaint();
}

void Canvas::setBSplineShadedVisibility(bool visible)
{
    emit setVisibilityOfBSplineSurfaceShaded(visible);
    repaint();
}

void Canvas::setModelVisibility(bool visible)
{
    modelVisible = visible;
    if (suzanne->isLoaded())
    {
        emit setLcdNumberValue(suzanne->getVerticesCount() / 3);
    }

    repaint();
}

void Canvas::loadModel(const QString &filePath)
{
    suzanne->reload(filePath);
    emit setLcdNumberValue(suzanne->getVerticesCount() / 3);

    reinitialiseModel();
    repaint();
}

void Canvas::setBezierVisibility(bool visible)
{
    emit setVisibilityOfBezierCurve(visible);
    if (visible)
    {
        bezierCurve->calculatePoints(controlPoints);
    }
    repaint();
}

void Canvas::setRationalBezierVisibility(bool visible)
{
    emit setVisibilityOfRationalBezierCurve(visible);
    if (visible)
    {
        bezierCurve->calculateRationalPoints(controlPoints, controlPointsWeights);
    }
    repaint();
}

void Canvas::setNurbsVisibility(bool visible)
{
    emit setVisibilityOfNurbsCurve(visible);
    if (visible)
    {
        bSplineCurve->calculateNURPoints(controlPoints, controlPointsWeights);
    }
    repaint();
}

void Canvas::setBSplineVisibility(bool visible)
{
    emit setVisibilityOfBSplineCurve(visible);
    if (visible)
    {
        bSplineCurve->calculatePoints(controlPoints);
    }
    repaint();
}

void Canvas::setBSplineSurfaceVisibility(bool visible)
{
    emit setVisibilityOfBSplineSurface(visible);
    repaint();
}

void Canvas::setBSplineSurfaceControlNetVisibility(bool visible)
{
    emit setVisibilityOfBSplineSurfaceControlNet(visible);
    repaint();
}

void Canvas::keyPressEvent(QKeyEvent *e)
{
    if (e->key() & Qt::Key_S)
    {
        qDebug() << "s" << "\n";
    }

    if (bSplineSurface->getIsVisible())
    {
        // if ((grabbedPointIndexJ != -1) && (grabbedPointIndex != -1))
        {
            if (e->key() & Qt::Key_P)
            {
                surfaceControlPoints[grabbedPointIndex][grabbedPointIndexJ].setZ(surfaceControlPoints[grabbedPointIndex][grabbedPointIndexJ].x()+2);

                translatedSurfaceControlPoints = bSplineSurface->translateSurfaceControlPoints(surfaceControlPoints);

                bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, surfaceDensity);
            }
            else if (e->key() & Qt::Key_M)
            {
                qDebug() << "-" ;

                surfaceControlPoints[grabbedPointIndex][grabbedPointIndexJ].setZ(surfaceControlPoints[grabbedPointIndex][grabbedPointIndexJ].x()-2);
                translatedSurfaceControlPoints = bSplineSurface->translateSurfaceControlPoints(surfaceControlPoints);
                bSplineSurface->calculate(surfaceControlPoints, surfaceControlPointsWeights, surfaceDensity);
            }
        }
    }
    QOpenGLWidget::keyPressEvent(e);
}

void Canvas::setAddingControlPoints(bool adding)
{
    if (adding )
    {
        radiobuttonMode = 0;
    }
    else
    {
        radiobuttonMode = 1;
    }
}

void Canvas::setMoving(bool moving)
{
    if (moving)
    {
        radiobuttonMode = 1;
    }
    else
    {
        radiobuttonMode = 0;
    }
    update();
}
