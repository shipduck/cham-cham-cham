#include "stdafx.h"
#include "joystick_input_event.h"

using namespace irr;
using namespace core;

/*
JoystickInputEvent::JoystickInputEvent()
	: Device(nullptr),
	SupportJoystick(false),
	XMovement(0.0f),
	YMovement(0.0f),
	XView(0.0f),
	YView(0.0f),
	Moved(false)
{
}
bool JoystickInputEvent::OnEvent(const irr::SEvent& event)
{
	if(event.EventType != irr::EET_JOYSTICK_INPUT_EVENT) {
		return false;
	}

	if(event.JoystickEvent.Joystick == 0) {
		return false;
	}

	return OnEvent(event.JoystickEvent);
}
bool JoystickInputEvent::OnEvent(const irr::SEvent::SJoystickEvent &event)
{
	JoystickState = event;
	return true;
}

void JoystickInputEvent::update()
{
	if(JoystickInfo.size() == 0) {
		return;
	}

	XView = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_R] / 32767.f;
	YView = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_U] / 32767.f;

	YView = 0.0f; //temp

	const f32 DEAD_ZONE = 0.20f;

	if(fabs(XView) < DEAD_ZONE) {
		XView = 0.0f;
	}
	if(fabs(YView) < DEAD_ZONE) {
		YView = 0.0f;
	}
}

float JoystickInputEvent::getLeftRightRotation() const
{
	return XView;
}
float JoystickInputEvent::getUpDownRotation() const
{
	return YView;
}
*/