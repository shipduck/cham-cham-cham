#include "stdafx.h"
#include "joystick_input_event.h"

using namespace irr;
using namespace core;

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

void JoystickInputEvent::setDevice(irr::IrrlichtDevice* device)
{
	Device = device;
}
void JoystickInputEvent::activateJoystickEvent()
{
	SupportJoystick = Device->activateJoysticks(JoystickInfo);
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

void JoystickInputEvent::showInfo() const
{
	if(SupportJoystick) {
		std::cout << "Joystick support is enabled and " << JoystickInfo.size();
		std::cout<< "joystick(s) are present." << std::endl;

		for(size_t joystick = 0 ; joystick < JoystickInfo.size() ; ++joystick) {
			std::cout << "Joystick " << joystick << ":" << std::endl;
			std::cout << "\tName: " << JoystickInfo[joystick].Name.c_str() << std::endl;
			std::cout << "\tAxes: " << JoystickInfo[joystick].Axes << std::endl;
			std::cout << "\tButtons: " << JoystickInfo[joystick].Buttons << std::endl;
			std::cout << "\tHat is: ";

			switch(JoystickInfo[joystick].PovHat) {
			case SJoystickInfo::POV_HAT_PRESENT:
				std::cout << "present" << std::endl;
				break;
			case SJoystickInfo::POV_HAT_ABSENT:
				std::cout << "absent" << std::endl;
				break;
			case SJoystickInfo::POV_HAT_UNKNOWN:
				std::cout << "unknown" << std::endl;
				break;
			}
		}
	} else {
		std::cout << "Joystick support is not enabled" << std::endl;
	}
}

const irr::SEvent::SJoystickEvent& JoystickInputEvent::getJoystickState() const
{
	return JoystickState;
}
const irr::core::array<irr::SJoystickInfo>& JoystickInputEvent::getJoystickInfo() const
{
	return JoystickInfo;
}