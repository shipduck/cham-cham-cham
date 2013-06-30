// Ŭnicode please 
#pragma once

#include "irrConsole/console.h"

class ConsoleEventReceiver : public irr::IEventReceiver {
public:
	bool OnEvent(const irr::SEvent& event);
};

void drawConsoleCaptions(irr::IrrlichtDevice *device);
bool onConsoleEvent(const irr::SEvent &event);
void setUpConsole(irr::IrrlichtDevice *device);

extern IC_Console gConsole;
