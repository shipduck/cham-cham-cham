// Ŭnicode please 
#pragma once
#include "util/event_receiver_manager.h"

class ConsoleEventReceiver : public ICustomEventReceiver {
public:
	virtual bool OnEvent(const irr::SEvent& event);
	virtual bool OnEvent(const SHeadTrackingEvent &evt) { return false; }
};
