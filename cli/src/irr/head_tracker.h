// Ŭnicode please 
#pragma once
#include <OVR.h>

struct SHeadTrackingEvent {
	SHeadTrackingEvent() : yaw(0), pitch(0), roll(0), success(true) {}
	SHeadTrackingEvent(float yaw, float pitch, float roll)
		: yaw(yaw), pitch(pitch), roll(roll), success(true) {}
	float yaw;
	float pitch;
	float roll;
	bool success;

	static const SHeadTrackingEvent &FailEvent();
};

class HeadTracker {
public:
	HeadTracker();
	~HeadTracker();

	bool startUp();
	bool shutDown();
	SHeadTrackingEvent getValue() const;
	bool isConnected() const;

private:
	OVR::Ptr<OVR::DeviceManager> Manager;
	OVR::Ptr<OVR::HMDDevice> HMD;
	OVR::Ptr<OVR::SensorDevice> Sensor;
	OVR::SensorFusion FusionResult;
	OVR::HMDInfo HmdInfo;
};

extern HeadTracker *g_headTracker;