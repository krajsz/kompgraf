#ifndef LEAPHANDLER_H
#define LEAPHANDLER_H

#include <Leap.h>

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QDebug>

class LeapHandler : public QObject
{
    Q_OBJECT
public:
    explicit LeapHandler(QObject *parent = 0);

private:

    Leap::Vector handDirection;
    Leap::Frame frameBefore;
    Leap::Vector handPositionDifference;
    Leap::Vector tipDifference;

    int framecounter = 0;

Q_SIGNALS:
    void emithandDirection(const Leap::Vector& dir);
    void emithandVelocity(const Leap::Vector& velocity);

    void emitHandRotation(const Leap::Vector& rotationVector);

    void emithandDifference(const Leap::Vector& difference);
    void emitIsConnected(bool connected);


public Q_SLOTS:
    void processFrame(const Leap::Frame& frame);
    void isConnected(bool connected);
};

#endif // LEAPHANDLER_H
