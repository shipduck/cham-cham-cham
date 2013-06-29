#pragma once

class InputEvent : public irr::IEventReceiver
{
public:
	virtual ~InputEvent() {};

	virtual bool OnEvent(const irr::SEvent& event) override = 0;

	virtual void update() = 0;

	virtual float getHorizontalMovement() const = 0;
	virtual float getVerticalMovement() const = 0;

	virtual float getLeftRightRotation() const = 0;
	virtual float getUpDownRotation() const = 0;

	// More action keys can be added
	//virtual bool isJumpKeyPressed() const = 0;
	//virtual bool isActionKeyPressed() const = 0;
private:

};