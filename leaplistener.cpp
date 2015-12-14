#include "leaplistener.h"

LeapListener::LeapListener()
{
}

void LeapListener::onConnect(const Leap::Controller &controller)
{
    qDebug() << Q_FUNC_INFO;

    controller.enableGesture(Leap::Gesture::TYPE_SWIPE);

}

bool LeapListener::isConnected() const
{
    return this->connected;
}

void LeapListener::onFocusGained(const Leap::Controller &controller)
{
    Q_UNUSED(controller)
    qDebug() << Q_FUNC_INFO;
}

void LeapListener::onFrame(const Leap::Controller &controller)
{
    if (controller.isConnected())
    {
        if (! connected)
        {
            connected = true;
            handler->isConnected(true);
        }

        Leap::Frame frame = controller.frame();
        if (frame.isValid())
        {
            handler->processFrame(frame);
            //qDebug() << "Processing frames..";
        }
    }
    else
    {
        qDebug() << "Controller is not connected!";
    }
}

void LeapListener::onInit(const Leap::Controller &controller)
{
    qDebug() << Q_FUNC_INFO;
    handler = new LeapHandler();

    if (controller.isConnected())
    {
        handler->isConnected(true);
        qDebug() << "Controller connected!";

        connected = true;
    }
    else
    {
        handler->isConnected(false);

        connected = false;
        qDebug() << "Controller not connected!";

    }
}
