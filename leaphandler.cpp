#include "leaphandler.h"

LeapHandler::LeapHandler(QObject *parent) : QObject(parent)
{
}

void LeapHandler::processFrame(const Leap::Frame &frame)
{
    Leap::HandList hands = frame.hands();
    Leap::Hand hand = hands.leftmost();
    Leap::Finger finger = hand.fingers().frontmost();

    framecounter++;

    if (framecounter == 200)
    {
        framecounter = 0;

    }
    qDebug() << framecounter;

    if (! framecounter)
    {
        if (hand.isValid())
        {
            qDebug() << "Setting frameBefore..";
            frameBefore = frame;
        }
    }

    if (frameBefore.isValid() && !framecounter)
    {
        float xRot = frame.rotationAngle(frameBefore, Leap::Vector::xAxis());
        float yRot = frame.rotationAngle(frameBefore, Leap::Vector::yAxis());
        float zRot = frame.rotationAngle(frameBefore, Leap::Vector::zAxis());

        float rot = frame.rotationAngle(frameBefore);
        Leap::Vector rotationVector(xRot, yRot, zRot);

        qDebug() << "Rotated! " << rotationVector.x << " " << rotationVector.y << " " << rotationVector.z;
        emit emitHandRotation(rotationVector);
    }

    if (finger.type() == Leap::Finger::TYPE_INDEX)
    {
        emit emithandVelocity(finger.tipVelocity().normalized());
    }

    emit emithandDirection(hands.frontmost().direction());

}

void LeapHandler::isConnected(bool connected)
{
    emit emitIsConnected(connected);
}
