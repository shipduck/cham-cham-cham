// Ŭnicode please

#pragma once

class JoystickEventReceiver : public irr::IEventReceiver
{
public:
	JoystickEventReceiver();

	bool OnEvent(const irr::SEvent& event) override;
	
	void update();
	void rotateCamera(irr::scene::ISceneNode* node, irr::u32 ms);
		
	irr::f32 getXMovement() const;
	irr::f32 getYMovement() const;

	irr::f32 getXView() const;
	irr::f32 getYView() const;

	void setDevice(irr::IrrlichtDevice* device);
	void setCursor(irr::gui::ICursorControl* cursor);
	
	const irr::SEvent::SJoystickEvent & getJoystickState() const;
private:
	irr::IrrlichtDevice* Device;
	irr::gui::ICursorControl* Cursor;
	irr::SEvent::SJoystickEvent JoystickState;

	irr::f32 XMovement;
	irr::f32 YMovement;

	irr::f32 XView;
	irr::f32 YView;

	bool Moved;
};