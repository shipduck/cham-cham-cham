#pragma once

#include "input_event.h"

/*
class JoystickInputEvent
{
public:
	JoystickInputEvent();

	void setDevice(irr::IrrlichtDevice* device);
	void activateJoystickEvent();

	bool OnEvent(const irr::SEvent& event) override;
	bool OnEvent(const irr::SEvent::SJoystickEvent &evt);
	bool OnEvent(const HeadTrackingEvent &evt) override { return false;}

	float getLeftRightRotation() const override;
	float getUpDownRotation() const override;

	const irr::SEvent::SJoystickEvent& getJoystickState() const;
	const irr::core::array<irr::SJoystickInfo>& getJoystickInfo() const;
private:
	irr::IrrlichtDevice* Device;
	irr::SEvent::SJoystickEvent JoystickState;
	irr::core::array<irr::SJoystickInfo> JoystickInfo;
	bool SupportJoystick;

	irr::f32 XView;
	irr::f32 YView;

	bool Moved;
};
*/