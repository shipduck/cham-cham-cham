// Ŭnicode please 
#pragma once

#include "util/event_receiver_manager.h"

class HMDEventReceiver : public ICustomEventReceiver {
public:
	HMDEventReceiver() : supportHMD_(false) {}
	virtual ~HMDEventReceiver() {}
	virtual bool OnEvent(const irr::SEvent &evt);
	virtual bool OnEvent(const SHeadTrackingEvent &evt) { return false; }
	bool isSupportHMD() const { return supportHMD_; }

private:
	bool supportHMD_;
};

extern HMDEventReceiver *g_hmdEventReceiver;