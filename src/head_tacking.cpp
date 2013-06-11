// Ŭnicode please 
#include "head_tracking.h"

using namespace OVR;

CHeadTracking::CHeadTracking() 
{
}

CHeadTracking::~CHeadTracking() 
{
	shutDown(); 
}

bool CHeadTracking::startUp()
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
}

bool CHeadTracking::shutDown()
{
	Sensor.Clear();
    HMD.Clear();
    Manager.Clear();

    OVR::System::Destroy();
	return true;
}

bool CHeadTracking::getValue(float *yaw, float *pitch, float *roll)
{
	if(Sensor == nullptr) {
		*yaw = 0;
		*pitch = 0;
		*roll = 0;
		return false;
	}

	// *** Per Frame
    // Get orientation quaternion to control view
    Quatf q = FusionResult.GetOrientation();

    // Create a matrix from quaternion,
    // where elements [0][0] through [3][3] contain rotation.
    Matrix4f bodyFrameMatrix(q); 

    // Get Euler angles from quaternion, in specified axis rotation order.
    q.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(yaw, pitch, roll);

	return true;
}

bool CHeadTracking::isConnected() const
{
	if(Sensor == nullptr) {
		return false;
	}
	return true;
}