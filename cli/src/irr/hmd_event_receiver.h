// Ŭnicode please 
#pragma once

#include "util/event_receiver_manager.h"

class HMDEventReceiver : public ICustomEventReceiver {
public:
	HMDEventReceiver();
	virtual ~HMDEventReceiver() {}
	virtual bool OnEvent(const irr::SEvent &evt);
	virtual bool OnEvent(const SHeadTrackingEvent &evt) { return false; }
	bool isSupportHMD() const { return (supportHMD_ > 0); }

private:
	int &supportHMD_;
};