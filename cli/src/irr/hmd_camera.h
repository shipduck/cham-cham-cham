// Ŭnicode please
#pragma once

#include "util/event_receiver_manager.h"
#include "game/input_event.h"

class AbstractHMDCameraEventReceiver : public ICustomEventReceiver {
public:
	AbstractHMDCameraEventReceiver(irr::scene::ICameraSceneNode *cam, float rotateSpeed, float moveSpeed);
	virtual ~AbstractHMDCameraEventReceiver();

public:
	irr::scene::ICameraSceneNode *getCamera() { return cam_; }
	MoveEvent getMoveEvent() const;
	LookEvent getLookEvent() const;

	virtual bool OnEvent(const irr::SEvent &evt);
	virtual bool OnEvent(const SHeadTrackingEvent &evt) { return false; }

public:
	virtual void update(int ms) = 0;

private:
	void onEvent(const irr::SEvent::SJoystickEvent &evt);
	void onEvent(const irr::SEvent::SKeyInput &evt);
	void onEvent(const irr::SEvent::SMouseInput &evt);

public:
	float rotateSpeed;
	float moveSpeed;

private:
	irr::scene::ICameraSceneNode *cam_;

	MoveEvent keyboardMoveEvent_;
	MoveEvent joystickMoveEvent_;

	LookEvent mouseLookEvent_;
	LookEvent joystickLookEvent_;
    
    ButtonEvent buttonEvent_;
    //keyboard down/up 추적. 이벤트 들어올때마다 계속 저장해서 이벤트 재구성하기
    bool keyDownStatus_[KeyMapping::kKeyTypeCount];
};

// 처다보는방향과 쏘는 방향이 다를수있도록 만든 카메라
class HeadFreeCameraEventReceiver : public AbstractHMDCameraEventReceiver {
public:
	HeadFreeCameraEventReceiver(irr::scene::ICameraSceneNode *cam, float rotateSpeed=1.0f, float moveSpeed=1.0f);
	virtual ~HeadFreeCameraEventReceiver();
	
	virtual void update(int ms);

public:
	irr::f32 getHorizontalRotate() const { return horizontalRotate_; }
	irr::f32 getVerticalRotate() const { return verticalRotate_; }

private:
	//카메라이동관련. 일단 하드코딩
	irr::f32 horizontalRotate_;
	irr::f32 verticalRotate_;
};

class HeadAttachCameraEventReceiver : public AbstractHMDCameraEventReceiver {
public:
	HeadAttachCameraEventReceiver(irr::scene::ICameraSceneNode *cam, float rotateSpeed=1.0f, float moveSpeed=1.0f);
	virtual ~HeadAttachCameraEventReceiver();

	virtual void update(int ms);
private:
	float rot_;
};