// Ŭnicode please 
#pragma once

#include "util/event_receiver_manager.h"

class FingerDirectionEvent {
public:
	enum {
		kNone,
		kLeft,
		kRight,
		kUp,
		kDown,
	};

	FingerDirectionEvent() : value(kNone) {}
	FingerDirectionEvent(int val) : value(val) {}

	static FingerDirectionEvent up() { return FingerDirectionEvent(kUp); }
	static FingerDirectionEvent down() { return FingerDirectionEvent(kDown); }
	static FingerDirectionEvent left() { return FingerDirectionEvent(kLeft); }
	static FingerDirectionEvent right() { return FingerDirectionEvent(kRight); }

	bool operator==(const FingerDirectionEvent &o) const;
	bool operator!=(const FingerDirectionEvent &o) const;

	bool isValid() const { return value != kNone; }

	int value;

	static int getDirectionCount();
};

class FingerDirectionEventReceiver : public ICustomEventReceiver {
public:
	FingerDirectionEventReceiver(bool keyboard, bool headtracker, bool leap);
	~FingerDirectionEventReceiver() {}

	virtual bool OnEvent(const irr::SEvent &evt);
	virtual bool OnEvent(const SHeadTrackingEvent &evt);
#ifdef USE_LEAP_MOTION
	virtual bool OnEvent(const SLeapMotionEvent &evt);
#endif
	
	bool isLeapReady() const { return isLeapReady_;}

	FingerDirectionEvent inputEvt;
	bool isLeapReady_;
	int leapReadyCooltime;
	int64_t leapTimeStamp;

private:
	bool useKeyboard_;
	bool useHeadTracker_;
	bool useLeapMotion_;
};