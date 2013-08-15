// Ŭnicode please 
#include "stdafx.h"
#include "hmd_camera.h"
#include "base/template_lib.h"
#include "base/lib.h"
#include "game/input_event.h"

#include "irr/debug_draw_manager.h"

using namespace irr;
using namespace std;
using boost::wformat;

inline bool isSetBinaryFlag(const irr::u32& flags, const irr::u32 offset) {
    return (flags & offset) == offset;
}

AbstractHMDCameraEventReceiver::AbstractHMDCameraEventReceiver(irr::scene::ICameraSceneNode *cam, float rotateSpeed, float moveSpeed)
	: cam_(cam),
	rotateSpeed(rotateSpeed),
	moveSpeed(moveSpeed)
{
	std::fill(keyDownStatus_, keyDownStatus_ + getArraySize(keyDownStatus_), false);
}
AbstractHMDCameraEventReceiver::~AbstractHMDCameraEventReceiver()
{
}

MoveEvent AbstractHMDCameraEventReceiver::getMoveEvent() const
{
	MoveEvent evt = keyboardMoveEvent_.merge(joystickMoveEvent_);
	return evt;
}
LookEvent AbstractHMDCameraEventReceiver::getLookEvent() const
{
	LookEvent evt = mouseLookEvent_.merge(joystickLookEvent_);
	return evt;
}

bool AbstractHMDCameraEventReceiver::OnEvent(const irr::SEvent &evt) 
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

void AbstractHMDCameraEventReceiver::onEvent(const irr::SEvent::SJoystickEvent &evt) 
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
	
	if(fabs(XView) < DEAD_ZONE) {
		XView = 0.0f;
	}
	if(fabs(YView) < DEAD_ZONE) {
		YView = 0.0f;
	}
	lookEvent.horizontalRotation = XView;
	//y는 패드 자체에서 방향이 반대로 나온다?
	lookEvent.verticalRotation = -YView;

	//TODO move가 실제로 발생했을떄만 이동처리하는거 구현할때 살려서 쓴다
	/*
	if(!core::equals(XMovement, 0.f) || !core::equals(YMovement, 0.f)) {
		Moved = true;
	} else {
		Moved = false;
	}
	*/
    for(const KeyMapping::joystickMap_t::value_type &joystickMapValue : Lib::keyMapping->getJoystickKeyMap()) {
        buttonEvent_.setButton(joystickMapValue.second, isSetBinaryFlag(evt.ButtonStates, joystickMapValue.first));
    }
}

void AbstractHMDCameraEventReceiver::onEvent(const irr::SEvent::SKeyInput &evt) 
{
	struct KeyCompFunctor {
		KeyCompFunctor(irr::EKEY_CODE key) : key(key) {}
		irr::EKEY_CODE key;

		bool operator()(const KeyMapData &a) {
			return (a.keyCode == key);
		}
	};
    
	//processing move event
    for(const KeyMapping::keyMap_t::value_type &keyMapList : Lib::keyMapping->getKeyMap()){
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

void AbstractHMDCameraEventReceiver::onEvent(const irr::SEvent::SMouseInput &evt)
{
	//printf("mouse\n");
	//TODO
	//마우스구현을 집어넣으면 게임 디버깅이 심히 힘들다
}

//////////////////////////////////////////////////////////////////////

HeadFreeCameraEventReceiver::HeadFreeCameraEventReceiver(irr::scene::ICameraSceneNode *cam, float rotateSpeed, float moveSpeed)
	: AbstractHMDCameraEventReceiver(cam, rotateSpeed, moveSpeed),
	horizontalRotate_(180),
	verticalRotate_(0)
{	
}

HeadFreeCameraEventReceiver::~HeadFreeCameraEventReceiver()
{
}

void HeadFreeCameraEventReceiver::update(int ms)
{
	auto cam = this->getCamera();

	MoveEvent moveEvt = getMoveEvent();
	LookEvent lookEvt = getLookEvent();

	horizontalRotate_ += lookEvt.horizontalRotation * rotateSpeed * ms;
	if(horizontalRotate_ > 180.0f) {
		horizontalRotate_ -= 360.0f;
	} else if(horizontalRotate_ < -180.0f) {
		horizontalRotate_ += 360.0f;
	}

	verticalRotate_ += lookEvt.verticalRotation * rotateSpeed * ms;
	const float maxVerticalRotation = 80.0f;
	if(verticalRotate_ < -maxVerticalRotation) {
		verticalRotate_ = -maxVerticalRotation;
	} else if(verticalRotate_ > maxVerticalRotation) {
		verticalRotate_ = maxVerticalRotation;
	}

	//그냥 생각없이 카메라를 돌리자. 오큘러스 대응은 렌더리쪽에서 알아서 처리될거다
	cam->setRotation(core::vector3df(verticalRotate_, horizontalRotate_, 0));
	
	//카메라 처다보는 방향으로 로직을 구현하면 오큘러스에서 설정한 값하고 꼬인다
	//v/h 값으로 따로 계산해야될듯
	core::vector3df pos = cam->getPosition();
	core::vector3df up(0, 1, 0);
	float targetX = -cos(core::degToRad(verticalRotate_)) * sin(core::degToRad(horizontalRotate_));
	float targetY = sin(core::degToRad(verticalRotate_));
	float targetZ = -cos(core::degToRad(verticalRotate_)) * cos(core::degToRad(horizontalRotate_));
	core::vector3df target(targetX, targetY, targetZ);
	irr::core::vector3df side = up.crossProduct(target);
	up = target.crossProduct(side);
	cam->setUpVector(up);

	const float moveFactor = moveSpeed * ms;
	auto moveDelta = moveFactor * moveEvt.forwardBackward * target;
	auto sideDelta = moveFactor * moveEvt.leftRight * side;
	auto nextPos = pos + moveDelta + sideDelta;
	cam->setPosition(nextPos);

	bool displayInfo = true;
	if(displayInfo) {
		irr::video::SColor white(255, 255, 255, 255);
		auto rotateMsg = (wformat(L"rotate : h=%.2f, v=%.2f") % horizontalRotate_ % verticalRotate_).str();
		g_debugDrawMgr->addString(core::vector2di(0, 100), rotateMsg, white);

		auto evtMsg = (wformat(L"evt : fb=%.2f, lr=%.2f") % moveEvt.forwardBackward % moveEvt.leftRight).str();
		g_debugDrawMgr->addString(core::vector2di(0, 100 + 14*1), evtMsg, white);

		auto targetMsg = (wformat(L"target : %.2f, %.2f, %.2f") % targetX % targetY % targetZ).str();
		g_debugDrawMgr->addString(core::vector2di(0, 100 + 14*2), targetMsg, white);

		auto sideMsg = (wformat(L"side : %.2f, %.2f, %.2f") % side.X % side.Y % side.Z).str();
		g_debugDrawMgr->addString(core::vector2di(0, 100 + 14*3), sideMsg, white);

		auto camPos = cam->getPosition();
		auto camPosMsg = (wformat(L"CamPos : %.2f, %.2f, %.2f") % camPos.X % camPos.Y % camPos.Z).str();
		g_debugDrawMgr->addString(core::vector2di(0, 100 + 14*4), camPosMsg, white);

		auto upVecMsg = (wformat(L"UpVec : %.2f, %.2f, %.2f") % up.X % up.Y % up.Z).str();
		g_debugDrawMgr->addString(core::vector2di(0, 100 + 14*5), upVecMsg, white);

	}
}

/////////////////////////////////////////////////////////////////////
HeadAttachCameraEventReceiver::HeadAttachCameraEventReceiver(irr::scene::ICameraSceneNode *cam, float rotateSpeed, float moveSpeed)
	: AbstractHMDCameraEventReceiver(cam, rotateSpeed, moveSpeed),
	rot_(0)
{
}

HeadAttachCameraEventReceiver::~HeadAttachCameraEventReceiver()
{
}

void HeadAttachCameraEventReceiver::update(int ms)
{
	auto cam = this->getCamera();
	auto cursor = Lib::device->getCursorControl();

	MoveEvent moveEvt = getMoveEvent();
	LookEvent lookEvt = getLookEvent();

	auto screenSize = Lib::driver->getScreenSize();
	int screenWidth = screenSize.Width;
	int screenHeight = screenSize.Height;

	float mouseX = (float)((screenWidth / 2) - cursor->getPosition().X);
	float mouseY = (float)((screenHeight / 2) - cursor->getPosition().Y);
	float moveFactor = moveSpeed * ms;

	cursor->setPosition((screenWidth/2), (screenHeight/2));
	cursor->setPosition(640, 400);
	irr::core::vector3df pos = cam->getPosition();
	irr::core::vector3df target = cam->getTarget() - pos;
	irr::core::vector3df up = cam->getUpVector();
	irr::core::vector3df side = target.crossProduct(up);
	irr::core::vector3df rotation = cam->getRotation();

	auto moveDelta = moveFactor * moveEvt.forwardBackward * target;
	auto sideDelta = moveFactor * moveEvt.leftRight * side;
	auto nextPos = pos + moveDelta + sideDelta;

	cam->setPosition(nextPos);
	cam->setRotation(irr::core::vector3df(rotation.X, rotation.Y - mouseX / 15.0f, rotation.Z + rot_));
}