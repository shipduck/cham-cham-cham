// Ŭnicode please 
#include "stdafx.h"
#include "head_tracker.h"

using namespace OVR;

HeadTracker headTrackerLocal;
HeadTracker *g_headTracker = &headTrackerLocal;

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
    Manager = *DeviceManager::Create();
    HMD     = *Manager->EnumerateDevices<HMDDevice>().CreateDevice();
    if (!HMD) {
        return false;
	}
    Sensor  = *HMD->GetSensor();

    // Get DisplayDeviceName, ScreenWidth/Height, etc..
    HMD->GetDeviceInfo(&HmdInfo);
    
    if (Sensor) {
        FusionResult.AttachToSensor(Sensor);
	}
	return true;
}

bool HeadTracker::shutDown()
{
	Sensor.Clear();
    HMD.Clear();
    Manager.Clear();

    OVR::System::Destroy();
	return true;
}

SHeadTrackingEvent HeadTracker::getValue() const
{
	if(Sensor == nullptr) {
		return SHeadTrackingEvent::FailEvent();
	}

	// *** Per Frame
    // Get orientation quaternion to control view
    Quatf q = FusionResult.GetOrientation();

    // Create a matrix from quaternion,
    // where elements [0][0] through [3][3] contain rotation.
    Matrix4f bodyFrameMatrix(q); 

    // Get Euler angles from quaternion, in specified axis rotation order.
	SHeadTrackingEvent evt;
    q.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&evt.yaw, &evt.pitch, &evt.roll);
	return evt;
}

bool HeadTracker::isConnected() const
{
	if(Sensor == nullptr) {
		return false;
	}
	return true;
}