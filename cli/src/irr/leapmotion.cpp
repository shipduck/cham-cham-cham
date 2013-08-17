// Ŭnicode please 
#include "stdafx.h"
#include "leapmotion.h"

using namespace Leap;

void IrrLeapListener::onFrame(const Controller& controller)
{
	const Frame frame = controller.frame();

	evt.frameId = frame.id();
	evt.fingers = frame.fingers();
	evt.hands = frame.hands();
	evt.tools = frame.tools();
	evt.gestures = frame.gestures();
}