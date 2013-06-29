#include "stdafx.h"
#include "joystick_input_event.h"

using namespace irr;
using namespace core;

JoystickInputEvent::JoystickInputEvent()
	: Device(nullptr),
	XMovement(0.0f),
	YMovement(0.0f),
	XView(0.0f),
	YView(0.0f),
	Moved(false)
{
}

void JoystickInputEvent::setDevice(irr::IrrlichtDevice* device)
{
	Device = device;
}
void JoystickInputEvent::activateJoystickEvent()
{
	Device->activateJoysticks(JoystickInfo);
}

bool JoystickInputEvent::OnEvent(const irr::SEvent& event)
{
	if (event.EventType == EET_JOYSTICK_INPUT_EVENT && event.JoystickEvent.Joystick == 0) {
		JoystickState = event.JoystickEvent;
	}

	return true;
}

void JoystickInputEvent::update()
{
	if(JoystickInfo.size() == 0) {
		return;
	}

	XMovement = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
	YMovement = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;

	XView = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_R] / 32767.f;
	YView = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_U] / 32767.f;

	YView = 0.0f; //temp

	const f32 DEAD_ZONE = 0.20f;

	if(fabs(XMovement) < DEAD_ZONE) {
		XMovement = 0.0f;
	}
	if(fabs(YMovement) < DEAD_ZONE) {
		YMovement = 0.0f;
	}

	if(fabs(XView) < DEAD_ZONE) {
		XView = 0.0f;
	}
	if(fabs(YView) < DEAD_ZONE) {
		YView = 0.0f;
	}

	const u16 povDegrees = JoystickState.POV / 100;

	if(povDegrees < 360) {
		if(povDegrees > 0 && povDegrees < 180) {
			XMovement = 1.f;
		} else if(povDegrees > 180) {
			XMovement = -1.f;
		}
		if(povDegrees > 90 && povDegrees < 270) {
			YMovement = -1.f;
		} else if(povDegrees > 270 || povDegrees < 90) {
			YMovement = +1.f;
		}
	}

	if(!core::equals(XMovement, 0.f) || !core::equals(YMovement, 0.f)) {
		Moved = true;
	} else {
		Moved = false;
	}
}

float JoystickInputEvent::getHorizontalMovement() const
{
	return XMovement;
}
float JoystickInputEvent::getVerticalMovement() const
{
	return YMovement;
}
float JoystickInputEvent::getLeftRightRotation() const
{
	return XView;
}
float JoystickInputEvent::getUpDownRotation() const
{
	return YView;
}

const irr::SEvent::SJoystickEvent& JoystickInputEvent::getJoystickState() const
{
	return JoystickState;
}
const irr::core::array<irr::SJoystickInfo>& JoystickInputEvent::getJoystickInfo() const
{
	return JoystickInfo;
}