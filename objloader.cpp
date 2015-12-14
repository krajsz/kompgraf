#include "objloader.h"

ObjLoader::ObjLoader(QObject *parent) : QObject(parent)
{

}

ModelData  ObjLoader::load(const QString& fileName)
{
    QFile input(fileName);
    ModelData data;

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

        QList<Face> faces;
        QList<Edge> edges;

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
                else if (lineList.size() > 4)
                {
                    if (lineList[2].contains('/') && lineList[1] == "") // f  1/..
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
                    else if (lineList[1].contains('/')) // f 1/..
                    {
                        if (lineList.size() == 5)
                        {
                            QList<int> tmpidxs;
                            qDebug() << "LineList size: " << lineList.size();
                            for (int i = 0; i < lineList.size(); ++i)
                            {
                                QStringList lineListList = lineList[i].split("/");
                                if (lineListList.size() == 3 )
                                {
                                    normalsIndices.push_back(lineListList[2].toInt()-1);
                                    //verticesIndices.push_back(lineListList[0].toInt()-1);
                                  //  qDebug() << "Pushing, v/vt/vn";

                                    tmpidxs.push_back(lineListList[0].toInt()-1);
                                }
                                else if (lineListList.size() == 2)
                                {
                                    //  verticesIndices.push_back(lineListList[0].toInt()-1);
                                 //   qDebug() << "Pushing, v/vt";
                                    tmpidxs.push_back(lineListList[0].toInt()-1);
                                }
                                else
                                {
                                    qDebug() << "wtf is dis";
                                }
                            }
                            qDebug() << tmpidxs.size();
                            verticesIndices.push_back(tmpidxs[0]);
                            verticesIndices.push_back(tmpidxs[1]);
                            verticesIndices.push_back(tmpidxs[2]);

                            verticesIndices.push_back(tmpidxs[0]);
                            verticesIndices.push_back(tmpidxs[2]);
                            verticesIndices.push_back(tmpidxs[3]);
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

            float_t maxX = std::numeric_limits<float_t>::min();
            float_t maxY = std::numeric_limits<float_t>::min();
            float_t maxZ = std::numeric_limits<float_t>::min();

            float_t minX = std::numeric_limits<float_t>::max();
            float_t minY = std::numeric_limits<float_t>::max();
            float_t minZ = std::numeric_limits<float_t>::max();

            for (int i = 0; i < tmpVertices.size(); ++i)
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

                if (tmpVertices[i].x() < minX)
                {
                    minX = tmpVertices[i].x();
                }
                if (tmpVertices[i].y() < minY)
                {
                    minY= tmpVertices[i].y();
                }
                if (tmpVertices[i].z() < minZ)
                {
                    minZ = tmpVertices[i].z();
                }
            }

            qDebug() << "X: " << maxX << " "<< minX;
            qDebug() << "Y: " << maxY << " "<< minY;
            qDebug() << "Z: " << maxZ << " "<< minZ;

            QVector3D bbox ( maxX - minX, maxY - minY, maxZ - minZ);

            qDebug() <<"box: " << bbox;
            for (int i = 0; i < tmpVertices.size(); ++i)
            {
                tmpVertices[i]/=bbox;
            }

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
                for (int i = 0; i < vertices.size()-3; i+=3)
                {
                    normals.push_back(QVector3D(QVector3D::normal(vertices[i], vertices[i+1], vertices[i+2])));
                    normals.push_back(QVector3D(QVector3D::normal(vertices[i], vertices[i+1], vertices[i+2])));
                    normals.push_back(QVector3D(QVector3D::normal(vertices[i], vertices[i+1], vertices[i+2])));
                }
            }

            Face face;
            Edge firstEdge;
            Edge secondEdge;
            Edge thirdEdge;

            for (int i = 0; i < vertices.size()-2; i+=3)
            {
                firstEdge =  Edge(vertices[i], vertices[i+1]);
                secondEdge = Edge(vertices[i+1], vertices[i+2]);
                thirdEdge =  Edge(vertices[i+2], vertices[i]);

                edges.push_back(firstEdge);
                edges.push_back(secondEdge);
                edges.push_back(thirdEdge);

                face = Face(vertices[i], vertices[i+1], vertices[i+2]);
                faces.push_back(face);
            }
            data.setNormals(normals);
            data.setNormalIndices(normalsIndices);
            data.setVertices(vertices);
            data.setVertexIndices(verticesIndices);
            data.setFaces(faces);
            data.setEdges(edges);
        }
        else
        {
            emit loaded(false);
        }
    }
    return data;
}
