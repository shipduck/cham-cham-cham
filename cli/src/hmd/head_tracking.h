// Ŭnicode please 
#pragma once
#include <OVR.h>

class HeadTracking {
public:
	HeadTracking();
	~HeadTracking();

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