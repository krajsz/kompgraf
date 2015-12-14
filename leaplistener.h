#ifndef LEAPLISTENER_H
#define LEAPLISTENER_H

#include "leaphandler.h"
#include <Leap.h>
#include <QObject>
#include <QDebug>


class LeapListener : public Leap::Listener
{

public:
    LeapListener();
    LeapHandler* handler;
    bool isConnected() const;

protected:
    void onConnect(const Leap::Controller& controller);
    void onInit(const Leap::Controller& controller);
    void onFocusGained(const Leap::Controller& controller);
    void onFrame(const Leap::Controller& controller);

private:
    Leap::HandList hands;
    bool connected;

};

#endif // LEAPLISTENER_H
