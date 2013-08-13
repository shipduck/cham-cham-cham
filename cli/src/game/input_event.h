// Ŭnicode please 
#pragma once

#include "game/key_mapping.h"

struct ButtonEvent {
    ButtonEvent() {
		std::fill(buttonState.begin(), buttonState.end(), false);
	}
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