// Ŭnicode please 
#include "stdafx.h"
#include "finger_direction_event.h"
#include "irr/leapmotion.h"
#include "irr/head_tracker.h"

using namespace irr;

int FingerDirectionEvent::getDirectionCount() 
{
	//좌우/좌우상하 중에서 선택
	//return 2; 
	return 4;
}

bool FingerDirectionEvent::operator==(const FingerDirectionEvent &o) const
{
	return (this->value == o.value);
}
bool FingerDirectionEvent::operator!=(const FingerDirectionEvent &o) const
{
	return !(*this == o);
}


FingerDirectionEventReceiver::FingerDirectionEventReceiver(bool keyboard, bool headtracker, bool leap)
	: useKeyboard_(keyboard),
	useHeadTracker_(headtracker),
	useLeapMotion_(leap),
	isLeapReady_(false),
	leapReadyCooltime(0),
	leapTimeStamp(0)
{
}
bool FingerDirectionEventReceiver::OnEvent(const irr::SEvent &evt)
{
	if(useKeyboard_ == false) {
		return false;
	}
	if(evt.EventType == irr::EET_KEY_INPUT_EVENT) {
		if(evt.KeyInput.PressedDown) {
			switch(evt.KeyInput.Key) {
			case KEY_LEFT:
				inputEvt = FingerDirectionEvent::left();
				return true;
				break;
			case KEY_RIGHT:
				inputEvt = FingerDirectionEvent::right();
				return true;
				break;
			case KEY_UP:
				if(FingerDirectionEvent::getDirectionCount() > 2) {
					inputEvt = FingerDirectionEvent::up();
					return true;
				} else {
					inputEvt.value = FingerDirectionEvent::kNone;
				}
				break;
			case KEY_DOWN:
				if(FingerDirectionEvent::getDirectionCount() > 2) {
					inputEvt = FingerDirectionEvent::down();
					return true;
				} else {
					inputEvt.value = FingerDirectionEvent::kNone;
				}
				break;
			default:
				return false;
			}
		} else {
			inputEvt.value = FingerDirectionEvent::kNone;
		}
	}
	return false;
}

bool FingerDirectionEventReceiver::OnEvent(const SHeadTrackingEvent &evt)
{
	if(useHeadTracker_ == false) {
		return false;
	}

	const float yawLimit = core::PI * 0.25f * 0.4f;
	const float pitchLimit = core::PI * 0.25f * 0.3f;
	if(evt.yaw > yawLimit) {
		//left
		inputEvt = FingerDirectionEvent::left();
		return true;
	} else if(evt.yaw < -yawLimit) {
		inputEvt = FingerDirectionEvent::right();
		//right
		return true;
	}
	if(FingerDirectionEvent::getDirectionCount() > 2) {
		if(evt.pitch > pitchLimit) {
			//up
			inputEvt = FingerDirectionEvent::up();
			return true;
		} else if(evt.pitch < -pitchLimit) {
			//down
			inputEvt = FingerDirectionEvent::down();
			return true;
		}
	}
	return false;
}

#ifdef USE_LEAP_MOTION
bool FingerDirectionEventReceiver::OnEvent(const SLeapMotionEvent &evt)
{
	if(useLeapMotion_ == false) {
		return false;
	}

	// Initializing
	if(leapTimeStamp == 0) {
		leapTimeStamp = evt.timestamp;
		return false;
	}

	if(evt.hands.count() != 0 && isLeapReady_ == false) {
		leapReadyCooltime += int(evt.timestamp - leapTimeStamp);
	}
	
	leapTimeStamp = evt.timestamp;

	if(evt.hands.count() == 0) {
		leapReadyCooltime = 0;
		isLeapReady_ = false;
		return false;
	}

	if(leapReadyCooltime >= 1000) {
		isLeapReady_ = true;

		auto gestures = evt.gestures;

		for(auto gesture : gestures) {
			if(gesture.type() == Leap::Gesture::TYPE_SWIPE) {
				const auto& swipe = static_cast<Leap::SwipeGesture>(gesture);
				auto vector = swipe.direction();

				// Float 비교 부정확성, 그리고 소수점 이하 비교는 필요하지 않기 떄문에
				// Int로 형변환해서 비교한다
				int x = 100 * int(vector.x);
				int y = 100 * int(vector.y);
				int absX = abs(x);
				int absY = abs(y);

				int absLargest = std::max(absX, absY);

				if(absLargest == x) {
					inputEvt = FingerDirectionEvent::right();
				}
				else if(absLargest == absX) {
					inputEvt = FingerDirectionEvent::left();
				}
				else if(absLargest == y) {
					inputEvt = FingerDirectionEvent::up();
				}
				else if(absLargest == absY) {
					inputEvt = FingerDirectionEvent::down();
				}

				return true;
			}
		}
	}

	return false;
}
#endif
