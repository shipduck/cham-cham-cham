// Ŭnicode please 
#include "stdafx.h"
#include "hmd_camera.h"
#include "base/template_lib.h"
#include "base/lib.h"
#include "game/input_event.h"

using namespace irr;
using namespace std;

inline bool isSetBinaryFlag(const irr::u32& flags, const irr::u32 offset) {
    return (flags & offset) == offset;
}


CameraEventReceiver::CameraEventReceiver(irr::scene::ICameraSceneNode *cam)
	: cam_(cam),
	horizontalRotate_(0),
	verticalRotate_(0),
	keyMapping_(new KeyMapping())
{
	std::fill(keyDownStatus_, keyDownStatus_ + getArraySize(keyDownStatus_), false);
}

CameraEventReceiver::~CameraEventReceiver()
{
}

MoveEvent CameraEventReceiver::getMoveEvent() const
{
	MoveEvent evt = keyboardMoveEvent_.merge(joystickMoveEvent_);
	return evt;
}
LookEvent CameraEventReceiver::getLookEvent() const
{
	LookEvent evt = mouseLookEvent_.merge(joystickLookEvent_);
	return evt;
}


bool CameraEventReceiver::OnEvent(const irr::SEvent &evt) 
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

void CameraEventReceiver::onEvent(const irr::SEvent::SJoystickEvent &evt) 
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
    //printf("%f\n", joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_Z>());
    //printf("%f\n", joystickDev.getAxisFloatValue<SEvent::SJoystickEvent::AXIS_V>());

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

void CameraEventReceiver::onEvent(const irr::SEvent::SKeyInput &evt) 
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

void CameraEventReceiver::onEvent(const irr::SEvent::SMouseInput &evt)
{
	//printf("mouse\n");
	//TODO
	//마우스구현을 집어넣으면 게임 디버깅이 심히 힘들다
}

void CameraEventReceiver::update(int ms)
{
	auto cam = this->getCamera();

	MoveEvent moveEvt = getMoveEvent();
	LookEvent lookEvt = getLookEvent();

	horizontalRotate_ += lookEvt.horizontalRotation;
	verticalRotate_ += lookEvt.verticalRotation;
	const float maxVerticalRotation = 88.0f;
	if(verticalRotate_ < -maxVerticalRotation) {
		verticalRotate_ = -maxVerticalRotation;
	} else if(verticalRotate_ > maxVerticalRotation) {
		verticalRotate_ = maxVerticalRotation;
	}

	//그냥 생각없이 카메라를 돌리자. 오큘러스 대응은 렌더리쪽에서 알아서 처리될거다
	cam->setRotation(core::vector3df(verticalRotate_, horizontalRotate_, 0));
	
	irr::core::vector3df pos = cam->getPosition();
	irr::core::vector3df target = cam->getTarget() - pos;
	irr::core::vector3df up = cam->getUpVector();
	//irr::core::vector3df side = target.crossProduct(up);
	irr::core::vector3df side = up.crossProduct(target);

	const float moveFactor = ((irr::f32)ms) / 10.0f;
	cam->setPosition(pos + moveFactor * moveEvt.forwardBackward * target + moveFactor * moveEvt.leftRight * side);
}