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

	// 매 프레임마다 호출하는 함수. 센서의 값을 저장해놓는다
	// 이를 따로 분리해놔야 다른곳에서 마음놓고 마구호출해도 성능문제가 없다
	void update();
	SHeadTrackingEvent getValue() const { return lastEvt_; }
	bool isConnected() const;

private:
	OVR::Ptr<OVR::DeviceManager> manager_;
	OVR::Ptr<OVR::HMDDevice> hmd_;
	OVR::Ptr<OVR::SensorDevice> sensor_;
	OVR::SensorFusion fusionResult_;
	OVR::HMDInfo hmdInfo_;

	SHeadTrackingEvent lastEvt_;
};
