#include "stdafx.h"
#include "input_event.h"

using namespace irr;

GameEventReceiver::GameEventReceiver() 
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
	const JoystickDevice &joystickDev = gEventReceiverMgr->getJoystickDev();
	const SJoystickInfo &joystickInfo = joystickDev.getJoystickInfo()[0];
	//TODO. joystick 이벤트는 매 프레임 polling으로 들어봐서 printf를 넣으면 도배가 되니까 생략
}

void GameEventReceiver::onEvent(const irr::SEvent::SKeyInput &evt) 
{
	//processing move event
	if(std::find(getForwardKeyList().begin(), getForwardKeyList().end(), evt.Key) != getForwardKeyList().end()) {
		if(evt.PressedDown) {
			moveEvent_.forwardBackward = 1.0f;
		} else {
			moveEvent_.forwardBackward = 0;
		}
	}
	if(std::find(getBackwardKeyList().begin(), getBackwardKeyList().end(), evt.Key) != getBackwardKeyList().end()) {
		if(evt.PressedDown) {
			moveEvent_.forwardBackward = -1.0f;
		} else {
			moveEvent_.forwardBackward = 0;
		}
	}
	if(std::find(getLeftKeyList().begin(), getLeftKeyList().end(), evt.Key) != getLeftKeyList().end()) {
		if(evt.PressedDown) {
			moveEvent_.leftRight = -1.0f;
		} else {
			moveEvent_.leftRight = 0;
		}
	}
	if(std::find(getRightKeyList().begin(), getRightKeyList().end(), evt.Key) != getRightKeyList().end()) {
		if(evt.PressedDown) {
			moveEvent_.leftRight = 1.0f;
		} else {
			moveEvent_.leftRight = 0;
		}
	}
}

void GameEventReceiver::onEvent(const irr::SEvent::SMouseInput &evt)
{
	printf("mouse\n");
	//TODO
}