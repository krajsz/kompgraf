#include "sceneobject.h"

SceneObject::SceneObject(const QString &fileName, QObject *parent) : QObject(parent)
{

    wtvMatrix = Utils::translate(QVector3D(100, 150, -150)) * Utils::scaleXYMatrix(250, 170) *
            Utils::translate(QVector3D(1,1,0));

    currentRotation =     currentRotation = Utils::rotateYmat(xRotation ) *
            Utils::rotateZmat(zRotation  ) ;

    transformationMatrix = wtvMatrix *currentRotation ;

    loader = new ObjLoader;
    connect(loader, &ObjLoader::loaded, this, &SceneObject::setLoaded);

    datas = loader->load(fileName);

    if (loaded)
    {
        qDebug() << fileName << " loaded!";

        qDebug() <<"Vertices count: " << getVerticesCount();
        qDebug() <<"Normals count: " << getNormalsCount();
        qDebug() <<"Vertex indices count: " << getVerticesIndicesSize();
        qDebug() <<"Normal indices count: " << getNormalsIndicesSize();
        qDebug() <<"Faces count: " << getFacesCount();
        qDebug() <<"Edges count: " << getEdgesCount();
    }
}

void SceneObject::destroyVao()
{
    if (vao.isCreated())
    {
        qDebug() << "vao destroyed!";
        vao.destroy();
    }
}

void SceneObject::subdivision(const int &iterations)
{

    for (int iteration = 0; iteration < iterations; ++iteration)
    {
        QList<Face> tmpFaces;
        QListIterator<Face> it(datas.getFaces());

        while (it.hasNext())
        {

        }
    }
}

const int SceneObject::getFacesCount()
{
    return datas.getFaces().size();
}

const int SceneObject::getEdgesCount()
{
    return datas.getEdges().size();
}

void SceneObject::reload(const QString &fileName)
{
    datas = loader->load(fileName);
    if (loaded)
    {
        qDebug() << fileName << " loaded!";

        qDebug() <<"Vertices count: " << getVerticesCount();
        qDebug() <<"Normals count: " << getNormalsCount();
        qDebug() <<"Vertex indices count: " << getVerticesIndicesSize();
        qDebug() <<"Normal indices count: " << getNormalsIndicesSize();
        qDebug() <<"Faces count: " << getFacesCount();
        qDebug() <<"Edges count: " << getEdgesCount();
    }
}

const QMatrix4x4& SceneObject::getTransformationMatrix() const
{
    return this->transformationMatrix;
}

void SceneObject::updateRotation(float_t rotX, float_t rotZ)
{
    xRotation = rotX;
    zRotation = rotZ;

    currentRotation = Utils::rotateYmat(xRotation/  2.0) * Utils::rotateZmat(zRotation /  2.0) ;
    transformationMatrix = wtvMatrix * currentRotation  ;
}

float_t SceneObject::getZrotation() const
{
    return zRotation;
}

float_t SceneObject::getXrotation() const
{
    return xRotation;
}

float_t SceneObject::getYrotation() const
{
    return yRotation;
}

bool SceneObject::isLoaded() const
{
    return loaded;
}

const QVector3D* SceneObject::getNormals()
{
    return datas.getNormals().toVector().constData();
}

const QList<unsigned int>& SceneObject::getVerticesIndices()
{
    return datas.getVertexIndices();
}

const QList<unsigned int>& SceneObject::getNormalsIndices()
{
    return datas.getNormalIndices();
}

const QVector3D *SceneObject::getVertices()
{
    return datas.getVertices().toVector().constData();
}

const int SceneObject::getNormalsIndicesSize()
{
    return datas.getNormalIndices().size();
}

const int SceneObject::getVerticesIndicesSize()
{
    return datas.getVertexIndices().size();
}

const int SceneObject::getVerticesCount()
{
    return datas.getVertices().size();
}

const int SceneObject::getNormalsCount()
{
    return datas.getNormals().size();
}

void SceneObject::setLoaded(bool loaded)
{
    this->loaded = loaded;
}

void SceneObject::setNormalIndices(const QList<unsigned int> &normalsIndices)
{
    this->datas.setNormalIndices( normalsIndices);
}

void SceneObject::setVerticesIndices(const QList<unsigned int> &verticesIndices)
{
    this->datas.setVertexIndices(verticesIndices);
}

void SceneObject::setVertices(const QList<QVector3D> &vertices)
{
    this->datas.setVertices(vertices);
}

void SceneObject::setNormals(const QList<QVector3D> &normals)
{
    this->datas.setNormals(normals);
}

void SceneObject::bindVao()
{
    vao.bind();
}

bool SceneObject::createVao()
{
    return vao.create();
}

void SceneObject::releaseVao()
{
    vao.release();
}
