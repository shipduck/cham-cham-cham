// Ŭnicode please 
#include "stdafx.h"
#include "irr_console.h"
#include "console_event_receiver.h"

bool ConsoleEventReceiver::OnEvent(const irr::SEvent& event) 
{
	return g_console->OnEvent(event);
}
