// Ŭnicode please 
#include "stdafx.h"
#include "input_event.h"

using namespace std;


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