// Ŭnicode please 
#include "stdafx.h"
#include "event_receiver_manager.h"
#include "game/joystick_input_event.h"

using namespace irr;
using namespace scene;

EventReceiverManager gEventReceiverMgrLocal;
EventReceiverManager *gEventReceiverMgr = &gEventReceiverMgrLocal;

EventReceiverManager::EventReceiverManager()
{
}
EventReceiverManager::~EventReceiverManager()
{
}
void EventReceiverManager::setUp(irr::IrrlichtDevice *dev)
{
	auto* joystick = new JoystickInputEvent;
	joystick->setDevice(dev);
	joystick->activateJoystickEvent();
	JoystickDev.reset(joystick);
}
void EventReceiverManager::shutDown()
{
	JoystickDev.reset(nullptr);

	LowPriorityReceiverList.clear();
	HighPriorityReceiverList.clear();
}
void EventReceiverManager::update(int ms)
{
	JoystickDev->update();
}

void EventReceiverManager::addReceiver(ICustomEventReceiver *receiver, int priority)
{
	SR_ASSERT(receiver != nullptr)

	PriorityReceiverListType *receiverList = nullptr;
	if(priority < 0) {
		receiverList = &HighPriorityReceiverList;
	} else {
		receiverList = &LowPriorityReceiverList;
	}
	PriorityEventReceiver tmp;
	tmp.Receiver.reset(receiver);
	tmp.Priority = priority;
	receiverList->push_back(std::move(tmp));

	std::sort(receiverList->begin(), receiverList->end(),
		[](const PriorityEventReceiver &a, const PriorityEventReceiver &b) {
			return a.Priority < b.Priority;
	});
}

bool EventReceiverManager::OnEvent(const HeadTrackingEvent &evt)
{
	for(auto &receiver : HighPriorityReceiverList) {
		receiver.Receiver->OnEvent(evt);
	}
	for(auto &receiver : LowPriorityReceiverList) {
		receiver.Receiver->OnEvent(evt);
	}
	return false;
}

bool EventReceiverManager::OnEvent(const irr::SEvent &evt)
{
	for(auto &receiver : HighPriorityReceiverList) {
		receiver.Receiver->OnEvent(evt);
	}

	switch(evt.EventType) {
	case irr::EET_JOYSTICK_INPUT_EVENT:
		JoystickDev->OnEvent(evt.JoystickEvent);
		break;
	default:
		break;
	}
	
	for(auto &receiver : LowPriorityReceiverList) {
		receiver.Receiver->OnEvent(evt);
	}

	return false;
}