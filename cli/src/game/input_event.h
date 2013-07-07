#pragma once

#include "util/event_receiver_manager.h"

struct InputEvent {
};

struct MoveEvent : public InputEvent {
	MoveEvent() : forwardBackward(0.0f), leftRight(0.0f) {}
	float forwardBackward;
	float leftRight;
};

struct LookEvent : public InputEvent {
	LookEvent() : leftRightRotation(0), upDownRotation(0) {}
	float leftRightRotation;
	float upDownRotation;
};


class GameEventReceiver : public ICustomEventReceiver {
public:
	GameEventReceiver();
	virtual ~GameEventReceiver() {}
	virtual bool OnEvent(const HeadTrackingEvent &evt);
	virtual bool OnEvent(const irr::SEvent &evt);
	void onEvent(const irr::SEvent::SJoystickEvent &evt);
	void onEvent(const irr::SEvent::SKeyInput &evt);
	void onEvent(const irr::SEvent::SMouseInput &evt);

public:
	const std::array<irr::EKEY_CODE, 2> &getForwardKeyList() const { return forwardKeyList_; }
	const std::array<irr::EKEY_CODE, 2> &getBackwardKeyList() const { return backwardKeyList_; }
	const std::array<irr::EKEY_CODE, 2> &getLeftKeyList() const { return leftKeyList_; }
	const std::array<irr::EKEY_CODE, 2> &getRightKeyList() const { return rightKeyList_; }

	const MoveEvent &getMoveEvent() const { return moveEvent_; }
	const LookEvent &getLookEvent() const { return lookEvent_; }

private:
	std::array<irr::EKEY_CODE, 2> forwardKeyList_;
	std::array<irr::EKEY_CODE, 2> backwardKeyList_;
	std::array<irr::EKEY_CODE, 2> leftKeyList_;
	std::array<irr::EKEY_CODE, 2> rightKeyList_;

	MoveEvent moveEvent_;
	LookEvent lookEvent_;
};

