#include "stdafx.h"
#include "input_event.h"

using namespace irr;

GameEventReceiver::GameEventReceiver() 
	: leftKeyDown_(false),
	rightKeyDown_(false),
	upKeyDown_(false),
	downKeyDown_(false)
{
	forwardKeyList_[0] = irr::KEY_UP;
	forwardKeyList_[1] = irr::KEY_KEY_W;

	backwardKeyList_[0] = irr::KEY_DOWN;
	backwardKeyList_[1] = irr::KEY_KEY_S;

	leftKeyList_[0] = irr::KEY_LEFT;
	leftKeyList_[1] = irr::KEY_KEY_A;

	rightKeyList_[0] = irr::KEY_RIGHT;
	rightKeyList_[1] = irr::KEY_KEY_D;
}

MoveEvent GameEventReceiver::getMoveEvent() const
{
	MoveEvent evt = keyboardMoveEvent_;
	if(joystickMoveEvent_.forwardBackward != 0) {
		evt.forwardBackward = joystickMoveEvent_.forwardBackward;
	}
	if(joystickMoveEvent_.leftRight != 0) {
		evt.leftRight = joystickMoveEvent_.leftRight;
	}
	return evt;
}
LookEvent GameEventReceiver::getLookEvent() const
{
	LookEvent evt = mouseLookEvent_;
	if(joystickLookEvent_.horizontalRotation != 0) {
		evt.horizontalRotation = joystickLookEvent_.horizontalRotation;
	}
	if(joystickLookEvent_.verticalRotation != 0) {
		evt.verticalRotation = joystickLookEvent_.verticalRotation;
	}
	return evt;
}

bool GameEventReceiver::OnEvent(const HeadTrackingEvent &evt)
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

	const JoystickDevice &joystickDev = gEventReceiverMgr->getJoystickDev();
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

	//processing move event
	if(std::find(getForwardKeyList().begin(), getForwardKeyList().end(), evt.Key) != getForwardKeyList().end()) {
		upKeyDown_ = evt.PressedDown;
	}
	if(std::find(getBackwardKeyList().begin(), getBackwardKeyList().end(), evt.Key) != getBackwardKeyList().end()) {
		downKeyDown_ = evt.PressedDown;
	}
	if(std::find(getLeftKeyList().begin(), getLeftKeyList().end(), evt.Key) != getLeftKeyList().end()) {
		leftKeyDown_ = evt.PressedDown;
	}
	if(std::find(getRightKeyList().begin(), getRightKeyList().end(), evt.Key) != getRightKeyList().end()) {
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
	printf("mouse\n");
	//TODO
}