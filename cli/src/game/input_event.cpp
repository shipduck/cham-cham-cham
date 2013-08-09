// Ŭnicode please
#include "stdafx.h"
#include "input_event.h"
#include "base/lib.h"
#include "base/template_lib.h"

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

void ButtonEvent::setButton(const KeyMapping::eKeyType button, const bool isDown)
{
    assert(button < KeyMapping::kKeyTypeCount - 4);
    if (button == KeyMapping::kUndefinedKey) {
        return;
    }
    if (buttonState[button] != isDown) {
        std::cout << button << " : " << (isDown?"down":"up") << std::endl; // for Debug
        
        buttonState[button] = isDown;
    }
}

ButtonEvent ButtonEvent::merge(const ButtonEvent &o) const
{
    ButtonEvent tmp = *this;
    
    for(uint8_t i(0); i < KeyMapping::kKeyTypeCount - 4; i++) {
        tmp.buttonState[i] |= o.buttonState[i];
    }
    
    return tmp;
}

inline bool isSetBinaryFlag(const u32& flags, const u32 offset) {
    return (flags & offset) == offset;
}

GameEventReceiver::GameEventReceiver()
: keyMapping_(new KeyMapping())
{
	std::fill(keyDownStatus_, keyDownStatus_ + getArraySize(keyDownStatus_), false);
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

	const float povDegrees = ((-evt.POV / 100.0) - 90) * 180 / irr::core::PI;
	if(povDegrees < 2 * irr::core::PI && povDegrees > 0) {
        XMovement = cosf(povDegrees);
        YMovement = sinf(povDegrees);
	}

	moveEvent.forwardBackward = YMovement;
	moveEvent.leftRight = XMovement;

	float XView = joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_R>();
	float YView = joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_U>();
    printf("%f\n", joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_Z>());
    printf("%f\n", joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_V>());

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
    for(const KeyMapping::joystickMap_t::value_type &joystickMapValue : keyMapping_->getJoystickKeyMap()) {
        buttonEvent_.setButton(joystickMapValue.second, isSetBinaryFlag(evt.ButtonStates, joystickMapValue.first));
    }
}

void GameEventReceiver::onEvent(const irr::SEvent::SKeyInput &evt) 
{
	struct KeyCompFunctor {
		KeyCompFunctor(irr::EKEY_CODE key) : key(key) {}
		irr::EKEY_CODE key;

		bool operator()(const KeyMapData &a) {
			return (a.keyCode == key);
		}
	};
    
	//processing move event
    for(const KeyMapping::keyMap_t::value_type &keyMapList : keyMapping_->getKeyMap()){
        auto keyItr = keyMapList.second.begin();
        auto keyEndItr = keyMapList.second.end();
        auto keyFound = std::find_if(keyItr, keyEndItr, KeyCompFunctor(evt.Key));
        if(keyFound != keyEndItr) {
            keyDownStatus_[keyMapList.first] = evt.PressedDown;
        }
    }

	MoveEvent moveEvt;
	if(keyDownStatus_[KeyMapping::kLeftKey]) {
		moveEvt.leftRight -= 1.0f;
	}
	if(keyDownStatus_[KeyMapping::kRightKey]) {
		moveEvt.leftRight += 1.0f;
	}
	if(keyDownStatus_[KeyMapping::kForwardKey]) {
		moveEvt.forwardBackward += 1.0f;
	}
	if(keyDownStatus_[KeyMapping::kBackwardKey]) {
		moveEvt.forwardBackward -= 1.0f;
	}
	keyboardMoveEvent_ = moveEvt;
}

void GameEventReceiver::onEvent(const irr::SEvent::SMouseInput &evt)
{
	//printf("mouse\n");
	//TODO
}