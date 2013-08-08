// Ŭnicode please
#pragma once

#include "util/event_receiver_manager.h"
#include "key_mapping.h"

struct ButtonEvent {
    ButtonEvent() : buttonState{{0}} {}
    std::array<bool, KeyMapping::kKeyTypeCount - 4> buttonState;
    void setButton(const KeyMapping::eKeyType button, const bool isDown);
    
    ButtonEvent merge(const ButtonEvent &o) const;
};

struct MoveEvent {
	MoveEvent() : forwardBackward(0.0f), leftRight(0.0f) {}
	float forwardBackward;
	float leftRight;

	MoveEvent merge(const MoveEvent &o) const;
};

struct LookEvent {
	LookEvent() : horizontalRotation(0), verticalRotation(0) {}
	float horizontalRotation;
	float verticalRotation;

	LookEvent merge(const LookEvent &o) const;
};


class GameEventReceiver : public ICustomEventReceiver {
public:
	GameEventReceiver();
	virtual ~GameEventReceiver();
	virtual bool OnEvent(const SHeadTrackingEvent &evt);
	virtual bool OnEvent(const irr::SEvent &evt);
	void onEvent(const irr::SEvent::SJoystickEvent &evt);
	void onEvent(const irr::SEvent::SKeyInput &evt);
	void onEvent(const irr::SEvent::SMouseInput &evt);

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
};

