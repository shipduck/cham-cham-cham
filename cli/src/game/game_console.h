// Ŭnicode please 
#pragma once

#include "irrConsole/console.h"

void drawConsoleCaptions(irr::IrrlichtDevice *device);
bool onConsoleEvent(const irr::SEvent &event);
void setUpConsole(irr::IrrlichtDevice *device);

extern IC_Console gConsole;
