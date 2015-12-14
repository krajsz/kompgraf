#include "objloader.h"

ObjLoader::ObjLoader(QObject *parent) : QObject(parent)
{

}

ModelData  ObjLoader::load(const QString& fileName)
{
    QFile input(fileName);
    ModelData data;

    bool bad_size = false;

    if (!input.open(QFile::ReadOnly))
    {
        qDebug() << "Opening " << fileName << " failed!";
        emit loaded(false);
    }
    else
    {
        QList<QVector3D> vertices;
        QList<QVector3D> normals;
        QList<unsigned int> verticesIndices;
        QList<unsigned int> normalsIndices;

        QList<QVector3D> tmpVertices;
        QList<QVector3D> tmpNormals;

        QTextStream stream(&input);
        QString line;
        QVector3D vertex;
        QVector3D normal;

        while (!stream.atEnd())
        {
            line = stream.readLine();
            QStringList lineList = line.split(" ");
            if (lineList.first() == "v")
            {
                if (lineList.size() > 4)
                {
                    vertex.setX(lineList[2].toDouble());
                    vertex.setY(lineList[3].toDouble());
                    vertex.setZ(lineList[4].toDouble());
                }
                else
                {
                    vertex.setX(lineList[1].toDouble());
                    vertex.setY(lineList[2].toDouble());
                    vertex.setZ(lineList[3].toDouble());
                }
                tmpVertices.push_back(vertex);
            }
            else if (lineList.first() == "vn")
            {
                normal.setX(lineList[1].toDouble());
                normal.setY(lineList[2].toDouble());
                normal.setZ(lineList[3].toDouble());

                tmpNormals.push_back(normal);
            }
            else if (lineList.first() == "f")
            {
                if (lineList.size() == 4)
                {
                    if (lineList[1].contains('/'))
                    {
                        for (int i = 0; i < lineList.size(); ++i)
                        {
                            QStringList lineListList = lineList[i].split("/");
                            if (lineListList.size() == 3)
                            {
                                normalsIndices.push_back(lineListList[2].toInt()-1);
                                verticesIndices.push_back(lineListList[0].toInt()-1);
                            }
                            else if (lineListList.size() == 2)
                            {
                                verticesIndices.push_back(lineListList[0].toInt()-1);
                            }
                        }
                    }
                    else
                    {
                        verticesIndices.push_back(lineList[1].toInt()-1);
                        verticesIndices.push_back(lineList[2].toInt()-1);
                        verticesIndices.push_back(lineList[3].toInt()-1);
                    }
                }
                else
                {
                    if (lineList[2].contains('/'))
                    {
                        for (int i = 0; i < lineList.size(); ++i)
                        {
                            QStringList lineListList = lineList[i].split("/");
                            if (lineListList.size() == 3 )
                            {
                                normalsIndices.push_back(lineListList[2].toInt()-1);
                                verticesIndices.push_back(lineListList[0].toInt()-1);
                            }
                            else if (lineListList.size() == 2)
                            {
                                verticesIndices.push_back(lineListList[0].toInt()-1);
                            }
                        }
                    }
                    else
                    {
                        verticesIndices.push_back(lineList[1].toInt()-1);
                        verticesIndices.push_back(lineList[2].toInt()-1);
                        verticesIndices.push_back(lineList[3].toInt()-1);
                    }
                }
            }
        }

        if (stream.atEnd())
        {
            emit loaded(true);
            /* if (bad_size)
            {
                float_t maxX = -1;
                float_t maxY = -1;
                float_t maxZ = -1;
                for (int i =0; i < tmpVertices.size(); ++i)
                {
                    if (tmpVertices[i].x() > maxX)
                    {
                        maxX = tmpVertices[i].x();
                    }
                    if (tmpVertices[i].y() > maxY)
                    {
                        maxY = tmpVertices[i].y();
                    }
                    if (tmpVertices[i].z() > maxZ)
                    {
                        maxZ = tmpVertices[i].z();
                    }
                }

                for (int i = 0; i < tmpVertices.size(); ++i)
                {
                    tmpVertices[i].setX( tmpVertices[i].x() / maxX);
                    tmpVertices[i].setY( tmpVertices[i].y() / maxY);
                    tmpVertices[i].setZ( tmpVertices[i].z() / maxZ);
                }
            }*/

            qDebug() << tmpVertices.size();

            for (int i = 0; i < verticesIndices.size(); ++i)
            {
                vertices.push_back(tmpVertices[verticesIndices[i]]);
            }

            if (normalsIndices.size() > 0)
            {
                for (int i = 0; i < normalsIndices.size(); ++i)
                {
                    normals.push_back(tmpNormals[normalsIndices[i]]);
                }
            }
            else
            {
                for (int i = 0; i < vertices.size() - 3; i+=3)
                {
                    normals.push_back(QVector3D(QVector3D::normal(vertices[i], vertices[i+1], vertices[i+2])));
                    normals.push_back(QVector3D(QVector3D::normal(vertices[i], vertices[i+1], vertices[i+2])));
                    normals.push_back(QVector3D(QVector3D::normal(vertices[i], vertices[i+1], vertices[i+2])));
                }
            }
            data.setNormals(normals);
            data.setNormalIndices(normalsIndices);
            data.setVertices(vertices);
            data.setVertexIndices(verticesIndices);
        }
        else
        {
            emit loaded(false);
        }
    }
    return data;
}
