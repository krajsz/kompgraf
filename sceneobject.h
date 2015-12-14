#ifndef OBJECT_H
#define OBJECT_H

#include "objloader.h"
#include "Utils.h"
#include "face.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>

class SceneObject : public QObject
{
    Q_OBJECT
public:
    explicit SceneObject(const QString& fileName, QObject *parent = 0);

    const QVector3D * getVertices() ;
    const QVector3D *getNormals();
    const QList<unsigned int>& getVerticesIndices() ;
    const QList<unsigned int>& getNormalsIndices() ;

    const int getVerticesCount() ;
    const int getNormalsCount() ;
    const int getVerticesIndicesSize() ;
    const int getNormalsIndicesSize() ;
    const int getFacesCount();
    const int getEdgesCount();

    bool isLoaded() const;

    float_t getZrotation() const;
    float_t getXrotation() const;
    float_t getYrotation() const;

    const QMatrix4x4&  getTransformationMatrix() const;
    void updateRotation(float_t rotX, float_t rotZ);

    void setVertices(const QList<QVector3D>& vertices);
    void setNormals(const QList<QVector3D>& normals);

    void setVerticesIndices(const QList<unsigned int>& verticesIndices);
    void setNormalIndices(const QList<unsigned int>& normalsIndices);

    void reload(const QString& fileName);

    void subdivision(const int& iterations);

    bool createVao();
    void bindVao();
    void destroyVao();
    void releaseVao();

Q_SIGNALS:
private:

    bool loaded;
    float_t xRotation =  -50.0;
    float_t zRotation =  30.0;
    float_t yRotation =  30.0;

    QMatrix4x4 transformationMatrix;
    QMatrix4x4 wtvMatrix;

    QMatrix4x4 currentRotation;

    ModelData datas;
    QOpenGLVertexArrayObject vao;
    ObjLoader* loader;
    QString fileName;
    QList<QVector3D> vertices;
    QList<QVector3D> normals;

    QList<unsigned int> verticesIndices;
    QList<unsigned int> normalsIndices;

public Q_SLOTS:
    void setLoaded(bool loaded);
};

#endif // OBJECT_H
