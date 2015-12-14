#ifndef MODELDATA_H
#define MODELDATA_H

#include <QList>
#include <QVector3D>

class ModelData
{
public:
    ModelData();
    ModelData& operator= (const ModelData& otherModel);

    QList<QVector3D>& getVertices() ;
    QList<QVector3D>& getNormals() ;
    QList<unsigned int>& getVertexIndices();
    QList<unsigned int>& getNormalIndices() ;

    bool hasData() const;

    void setVertices(const QList<QVector3D>& vertices);
    void setNormals(const QList<QVector3D>& normals);
    void setVertexIndices(const QList<unsigned int>& vertexIndices);
    void setNormalIndices(const QList<unsigned int>& normalIndices);

private:

    QList<QVector3D> vertices;
    QList<QVector3D> normals;
    QList<unsigned int> vertexIndices;
    QList<unsigned int> normalIndices;
};

#endif // MODELDATA_H
