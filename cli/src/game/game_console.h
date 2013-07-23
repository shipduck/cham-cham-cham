// Ŭnicode please 
#pragma once

#include "console/console.h"
#include "util/event_receiver_manager.h"

class ConsoleEventReceiver : public ICustomEventReceiver {
public:
	virtual bool OnEvent(const irr::SEvent& event);
	virtual bool OnEvent(const HeadTrackingEvent &evt) { return false; }
};

void drawConsoleCaptions(irr::IrrlichtDevice *device);
bool onConsoleEvent(const irr::SEvent &event);
void setUpConsole(irr::IrrlichtDevice *device);

extern IC_Console gConsole;
