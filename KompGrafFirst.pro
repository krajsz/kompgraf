#-------------------------------------------------
#
# Project created by QtCreator 2015-09-28T17:21:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KompGrafHomework
TEMPLATE =  app

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
        Utils.cpp \
    canvas.cpp \
    settingsdialog.cpp \
    shadersourceeditdialog.cpp \
    bsplinecurve.cpp \
     beziercurve.cpp \
    bsplinesurface.cpp \
    leaphandler.cpp \
    leaplistener.cpp \
    subdivisioncurve.cpp \
    sceneobject.cpp \
    objloader.cpp \
    modeldata.cpp \
    subdivision.cpp \
    edge.cpp \
    face.cpp

HEADERS  += mainwindow.h \
            Utils.h \
    canvas.h \
    settingsdialog.h \
    shadersourceeditdialog.h \
    bsplinecurve.h \
    beziercurve.h \
    bsplinesurface.h \
    leaphandler.h \
    leaplistener.h \
    subdivisioncurve.h \
    sceneobject.h \
    objloader.h \
    modeldata.h \
    subdivision.h \
    edge.h \
    face.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    shadersourceeditdialog.ui

RESOURCES += myresources.qrc

INCLUDEPATH +=-I/usr/include/qtleapmotion/
INCLUDEPATH += Leap/include
INCLUDEPATH += .

LIBS += -L../../QtLeapMotion/Leap/x64/ -lLeap

LIBS += -L/usr/lib -lLeap

LIBS += `pkg-config libqtleapmotion --libs`

DISTFILES += \
    flatshadedsurfacevertexshader.vert \
    flatshadedsurfacefragmentshader.frag \
    phongshadedsurfacevertexshader.vert \
    phongshadedsurfacefragmentshader.frag \
    computeshader.cs \
    surfacenetfragmentshader.frag \
    gouraudshadedsurfacefragmentshader.frag \
    gouraudshadedsurfacevertexshader.vert
