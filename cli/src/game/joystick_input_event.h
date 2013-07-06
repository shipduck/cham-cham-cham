#pragma once

#include "input_event.h"

class JoystickInputEvent final : public InputEvent
{
public:
	JoystickInputEvent();

	void setDevice(irr::IrrlichtDevice* device);
	void activateJoystickEvent();

	bool OnEvent(const irr::SEvent& event) override;
	bool OnEvent(const irr::SEvent::SJoystickEvent &evt);
	bool OnEvent(const HeadTrackingEvent &evt) override { return false;}

	void update() override;

	float getHorizontalMovement() const override;
	float getVerticalMovement() const override;

	float getLeftRightRotation() const override;
	float getUpDownRotation() const override;

	void showInfo() const;

	const irr::SEvent::SJoystickEvent& getJoystickState() const;
	const irr::core::array<irr::SJoystickInfo>& getJoystickInfo() const;
private:
	irr::IrrlichtDevice* Device;
	irr::SEvent::SJoystickEvent JoystickState;
	irr::core::array<irr::SJoystickInfo> JoystickInfo;
	bool SupportJoystick;

	irr::f32 XMovement;
	irr::f32 YMovement;

	irr::f32 XView;
	irr::f32 YView;

	bool Moved;
};