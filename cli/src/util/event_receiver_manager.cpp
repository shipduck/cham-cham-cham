// Ŭnicode please 
#include "stdafx.h"
#include "event_receiver_manager.h"

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
	JoystickDev.reset(new JoystickDevice(dev));
}
void EventReceiverManager::shutDown()
{
	JoystickDev.reset(nullptr);

	LowPriorityReceiverList.clear();
	HighPriorityReceiverList.clear();
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

JoystickDevice::JoystickDevice(irr::IrrlichtDevice *dev)
	: Device(dev), SupportJoystick(false)
{
	if(dev != nullptr) {
		SupportJoystick = Device->activateJoysticks(JoystickInfo);
	}
}
bool JoystickDevice::OnEvent(const irr::SEvent &evt)
{
	if(evt.EventType != irr::EET_JOYSTICK_INPUT_EVENT) {
		return false;
	}
	return OnEvent(evt.JoystickEvent);
}

bool JoystickDevice::OnEvent(const irr::SEvent::SJoystickEvent &evt)
{
	JoystickState = evt;
	return true;
}

void JoystickDevice::showInfo() const
{
	if(SupportJoystick) {
		std::cout << "Joystick support is enabled and " << JoystickInfo.size();
		std::cout<< "joystick(s) are present." << std::endl;

		for(size_t joystick = 0 ; joystick < JoystickInfo.size() ; ++joystick) {
			std::cout << "Joystick " << joystick << ":" << std::endl;
			std::cout << "\tName: " << JoystickInfo[joystick].Name.c_str() << std::endl;
			std::cout << "\tAxes: " << JoystickInfo[joystick].Axes << std::endl;
			std::cout << "\tButtons: " << JoystickInfo[joystick].Buttons << std::endl;
			std::cout << "\tHat is: ";

			switch(JoystickInfo[joystick].PovHat) {
			case SJoystickInfo::POV_HAT_PRESENT:
				std::cout << "present" << std::endl;
				break;
			case SJoystickInfo::POV_HAT_ABSENT:
				std::cout << "absent" << std::endl;
				break;
			case SJoystickInfo::POV_HAT_UNKNOWN:
				std::cout << "unknown" << std::endl;
				break;
			}
		}
	} else {
		std::cout << "Joystick support is not enabled" << std::endl;
	}
}