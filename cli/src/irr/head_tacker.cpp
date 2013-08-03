// Ŭnicode please 
#include "stdafx.h"
#include "head_tracker.h"

using namespace OVR;

const SHeadTrackingEvent &SHeadTrackingEvent::FailEvent()
{
	static bool init = false;
	static SHeadTrackingEvent evt;
	if(init == false) {
		init = true;
		evt.success = false;
	}
	return evt;
}

HeadTracker::HeadTracker() 
{
}

HeadTracker::~HeadTracker() 
{
}

bool HeadTracker::startUp()
{
	OVR::System::Init();

	// *** Initialization - Create the first available HMD Device
    manager_ = *DeviceManager::Create();
    hmd_ = *manager_->EnumerateDevices<HMDDevice>().CreateDevice();
    if (!hmd_) {
        return false;
	}
    sensor_ = *hmd_->GetSensor();

    // Get DisplayDeviceName, ScreenWidth/Height, etc..
    hmd_->GetDeviceInfo(&hmdInfo_);
    
    if (sensor_) {
        fusionResult_.AttachToSensor(sensor_);
	} else {
		lastEvt_ = SHeadTrackingEvent::FailEvent();
	}
	return true;
}

bool HeadTracker::shutDown()
{
	sensor_.Clear();
    hmd_.Clear();
    manager_.Clear();

    OVR::System::Destroy();
	return true;
}

void HeadTracker::update()
{
	if(sensor_ == nullptr) {
		return;
	}

	// *** Per Frame
    // Get orientation quaternion to control view
    Quatf q = fusionResult_.GetOrientation();

    // Create a matrix from quaternion,
    // where elements [0][0] through [3][3] contain rotation.
    Matrix4f bodyFrameMatrix(q); 

    // Get Euler angles from quaternion, in specified axis rotation order.
    q.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&lastEvt_.yaw, &lastEvt_.pitch, &lastEvt_.roll);
}

bool HeadTracker::isConnected() const
{
	if(sensor_ == nullptr) {
		return false;
	}
	return true;
}