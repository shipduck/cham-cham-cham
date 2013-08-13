// Ŭnicode please
#pragma once

#include "util/event_receiver_manager.h"
#include "game/input_event.h"

class CameraEventReceiver : public ICustomEventReceiver {
public:
	CameraEventReceiver(irr::scene::ICameraSceneNode *cam);
	virtual ~CameraEventReceiver();
	virtual bool OnEvent(const irr::SEvent &evt);
	virtual bool OnEvent(const SHeadTrackingEvent &evt) { return false; }
	void update(int ms);

private:
	void onEvent(const irr::SEvent::SJoystickEvent &evt);
	void onEvent(const irr::SEvent::SKeyInput &evt);
	void onEvent(const irr::SEvent::SMouseInput &evt);

public:
	irr::f32 getHorizontalRotate() const { return horizontalRotate_; }
	irr::f32 getVerticalRotate() const { return verticalRotate_; }

	irr::scene::ICameraSceneNode *getCamera() { return cam_; }

public:
	MoveEvent getMoveEvent() const;
	LookEvent getLookEvent() const;

private:
	MoveEvent keyboardMoveEvent_;
	MoveEvent joystickMoveEvent_;

	LookEvent mouseLookEvent_;
	LookEvent joystickLookEvent_;
    
    ButtonEvent buttonEvent_;
	std::unique_ptr<KeyMapping> keyMapping_;
    //keyboard down/up 추적. 이벤트 들어올때마다 계속 저장해서 이벤트 재구성하기
    bool keyDownStatus_[KeyMapping::kKeyTypeCount];

private:
	//카메라이동관련. 일단 하드코딩
	irr::f32 horizontalRotate_;
	irr::f32 verticalRotate_;

	irr::scene::ICameraSceneNode *cam_;
};
