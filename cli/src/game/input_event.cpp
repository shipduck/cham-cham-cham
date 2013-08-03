// Ŭnicode please
#include "stdafx.h"
#include "input_event.h"
#include "key_mapping.h"
#include "base/lib.h"

using namespace irr;


MoveEvent MoveEvent::merge(const MoveEvent &o) const
{
	MoveEvent tmp = *this;
	if(tmp.forwardBackward == 0.0f && o.forwardBackward != 0.0f) {
		tmp.forwardBackward = o.forwardBackward;
	}
	if(tmp.leftRight == 0.0f && o.leftRight != 0.0f) {
		tmp.leftRight = o.leftRight;
	}
	return tmp;
}

LookEvent LookEvent::merge(const LookEvent &o) const
{
	LookEvent tmp = *this;
	if(tmp.horizontalRotation == 0.0f && o.horizontalRotation != 0.0f) {
		tmp.horizontalRotation = o.horizontalRotation;
	}
	if(tmp.verticalRotation == 0.0f && o.verticalRotation != 0.0f) {
		tmp.verticalRotation = o.verticalRotation;
	}
	return tmp;
}

GameEventReceiver::GameEventReceiver() 
	: leftKeyDown_(false),
	rightKeyDown_(false),
	upKeyDown_(false),
	downKeyDown_(false),
	keyMapping_(new KeyMapping())
{
	
}

GameEventReceiver::~GameEventReceiver()
{
}

MoveEvent GameEventReceiver::getMoveEvent() const
{
	MoveEvent evt = keyboardMoveEvent_.merge(joystickMoveEvent_);
	return evt;
}
LookEvent GameEventReceiver::getLookEvent() const
{
	LookEvent evt = mouseLookEvent_.merge(joystickLookEvent_);
	return evt;
}

bool GameEventReceiver::OnEvent(const SHeadTrackingEvent &evt)
{
	//TODO
	return false;
}

bool GameEventReceiver::OnEvent(const irr::SEvent &evt) 
{
	switch(evt.EventType) {
	case EET_JOYSTICK_INPUT_EVENT:
		onEvent(evt.JoystickEvent);
		break;
	case EET_KEY_INPUT_EVENT:
		onEvent(evt.KeyInput);
		break;
	case EET_MOUSE_INPUT_EVENT:
		onEvent(evt.MouseInput);
		break;
	}
	return false;
}

void GameEventReceiver::onEvent(const irr::SEvent::SJoystickEvent &evt) 
{
	const float DEAD_ZONE = 0.20f;

	MoveEvent &moveEvent = joystickMoveEvent_;
	LookEvent &lookEvent = joystickLookEvent_;

	const JoystickDevice &joystickDev = Lib::eventReceiver->getJoystickDev();
	const SJoystickInfo &joystickInfo = joystickDev.getJoystickInfo()[0];

	float XMovement = joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_X>();
	float YMovement = -joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_Y>();

	if(fabs(XMovement) < DEAD_ZONE) {
		XMovement = 0.0f;
	}
	if(fabs(YMovement) < DEAD_ZONE) {
		YMovement = 0.0f;
	}

	const u16 povDegrees = evt.POV / 100;
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

	moveEvent.forwardBackward = YMovement;
	moveEvent.leftRight = XMovement;

	float XView = joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_R>();
	float YView = joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_U>();

	YView = 0.0f;	//위아래 보는거 구현하기전까지는 막아놓기
	if(fabs(XView) < DEAD_ZONE) {
		XView = 0.0f;
	}
	if(fabs(YView) < DEAD_ZONE) {
		YView = 0.0f;
	}
	lookEvent.horizontalRotation = XView;
	lookEvent.verticalRotation = YView;

	//TODO move가 실제로 발생했을떄만 이동처리하는거 구현할때 살려서 쓴다
	/*
	if(!core::equals(XMovement, 0.f) || !core::equals(YMovement, 0.f)) {
		Moved = true;
	} else {
		Moved = false;
	}
	*/
}

void GameEventReceiver::onEvent(const irr::SEvent::SKeyInput &evt) 
{
	MoveEvent &moveEvent = keyboardMoveEvent_;

	struct KeyCompFunctor {
		KeyCompFunctor(irr::EKEY_CODE key) : key(key) {}
		irr::EKEY_CODE key;

		bool operator()(const KeyMapData &a) {
			return (a.keyCode == key);
		}
	};

	//processing move event
	auto forwardIt = keyMapping_->getForwardKeyList().begin();
	auto forwardEndit = keyMapping_->getForwardKeyList().end();
	auto forwardFound = std::find_if(forwardIt, forwardEndit, KeyCompFunctor(evt.Key));
	if(forwardFound != forwardEndit) {
		upKeyDown_ = evt.PressedDown;
	}

	auto backwardIt = keyMapping_->getBackwardKeyList().begin();
	auto backwardEndit = keyMapping_->getBackwardKeyList().end();
	auto backwardFound = std::find_if(backwardIt, backwardEndit, KeyCompFunctor(evt.Key));
	if(backwardFound != backwardEndit) {
		downKeyDown_ = evt.PressedDown;
	}

	auto leftIt = keyMapping_->getLeftKeyList().begin();
	auto leftEndit = keyMapping_->getLeftKeyList().end();
	auto leftFound = std::find_if(leftIt, leftEndit, KeyCompFunctor(evt.Key));
	if(leftFound != leftEndit) {
		leftKeyDown_ = evt.PressedDown;
	}

	auto rightIt = keyMapping_->getRightKeyList().begin();
	auto rightEndit = keyMapping_->getRightKeyList().end();
	auto rightFound = std::find_if(rightIt, rightEndit, KeyCompFunctor(evt.Key));
	if(rightFound != rightEndit) {
		rightKeyDown_ = evt.PressedDown;
	}

	MoveEvent moveEvt;
	if(leftKeyDown_) {
		moveEvt.leftRight -= 1.0f;
	}
	if(rightKeyDown_) {
		moveEvt.leftRight += 1.0f;
	}
	if(upKeyDown_) {
		moveEvt.forwardBackward += 1.0f;
	}
	if(downKeyDown_) {
		moveEvt.forwardBackward -= 1.0f;
	}
	keyboardMoveEvent_ = moveEvt;
}

void GameEventReceiver::onEvent(const irr::SEvent::SMouseInput &evt)
{
	//printf("mouse\n");
	//TODO
}