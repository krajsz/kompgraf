#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "modeldata.h"

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QTextStream>
#include <QFile>
#include <QDebug>

class ObjLoader : public QObject
{
    Q_OBJECT
public:
    explicit ObjLoader(QObject *parent = 0);
    ModelData load(const QString& fileName);
Q_SIGNALS:
    void loaded(bool loaded);

public slots:
};

#endif // OBJLOADER_H
