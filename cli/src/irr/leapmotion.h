// Ŭnicode please 
#pragma once

#ifdef USE_LEAP_MOTION

#include <Leap.h>

struct SLeapMotionEvent
{
	int64_t frameId;
	Leap::FingerList fingers;
	Leap::HandList hands;
	Leap::ToolList tools;
	Leap::GestureList gestures;
};

class IrrLeapListener : public Leap::Listener
{
public:
	//virtual void onInit(const Leap::Controller&) override;
	//virtual void onConnect(const Leap::Controller&) override;
	//virtual void onDisconnect(const Leap::Controller&) override;
	//virtual void onExit(const Leap::Controller&) override;
	virtual void onFrame(const Leap::Controller&) override;
	//virtual void onFocusGained(const Leap::Controller&) override;
	//virtual void onFocusLost(const Leap::Controller&) override;

	const SLeapMotionEvent& getEvent() const { return evt;}
private:
	SLeapMotionEvent evt;
};

#endif