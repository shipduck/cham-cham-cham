// Ŭnicode please 
#pragma once
#include <OVR.h>

class HeadTracker {
public:
	HeadTracker();
	~HeadTracker();

	bool startUp();
	bool shutDown();
	bool getValue(float *yaw, float *pitch, float *roll);
	bool isConnected() const;

private:
	OVR::Ptr<OVR::DeviceManager> Manager;
	OVR::Ptr<OVR::HMDDevice> HMD;
	OVR::Ptr<OVR::SensorDevice> Sensor;
	OVR::SensorFusion FusionResult;
	OVR::HMDInfo HmdInfo;
};

struct SHeadTrackingEvent {
	SHeadTrackingEvent() : yaw(0), pitch(0), roll(0) {}
	SHeadTrackingEvent(float yaw, float pitch, float roll)
		: yaw(yaw), pitch(pitch), roll(roll) {}
	float yaw;
	float pitch;
	float roll;
};

extern HeadTracker *g_headTracker;