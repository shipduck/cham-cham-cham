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
	auto* joystick = new JoystickDevice();
	joystick->setDevice(dev);
	joystickDev_.reset(joystick);
}

void EventReceiverManager::shutDown()
{
	joystickDev_.reset(nullptr);

	lowPriorityReceiverList_.clear();
	highPriorityReceiverList_.clear();
}

ICustomEventReceiver *EventReceiverManager::addReceiver(ICustomEventReceiver *receiver, int priority)
{
	SR_ASSERT(receiver != nullptr)

	PriorityReceiverListType *receiverList = nullptr;
	if(priority < 0) {
		receiverList = &highPriorityReceiverList_;
	} else {
		receiverList = &lowPriorityReceiverList_;
	}
	PriorityEventReceiver tmp;
	tmp.receiver.reset(receiver);
	tmp.priority = priority;
	receiverList->push_back(std::move(tmp));

	std::sort(receiverList->begin(), receiverList->end(),
		[](const PriorityEventReceiver &a, const PriorityEventReceiver &b) {
			return a.priority < b.priority;
	});
	return receiver;
}

bool EventReceiverManager::OnEvent(const HeadTrackingEvent &evt)
{
	for(auto &receiver : highPriorityReceiverList_) {
		receiver.receiver->OnEvent(evt);
	}
	for(auto &receiver : lowPriorityReceiverList_) {
		receiver.receiver->OnEvent(evt);
	}
	return false;
}

bool EventReceiverManager::OnEvent(const irr::SEvent &evt)
{
	for(auto &receiver : highPriorityReceiverList_) {
		receiver.receiver->OnEvent(evt);
	}

	switch(evt.EventType) {
	case irr::EET_JOYSTICK_INPUT_EVENT:
		joystickDev_->OnEvent(evt.JoystickEvent);
		break;
	default:
		break;
	}
	
	for(auto &receiver : lowPriorityReceiverList_) {
		receiver.receiver->OnEvent(evt);
	}

	return false;
}

JoystickDevice::JoystickDevice()
	: device_(nullptr), supportJoystick_(false)
{
}

void JoystickDevice::setDevice(irr::IrrlichtDevice* device) 
{
	device_ = device; 
	if(device != nullptr) {
		supportJoystick_ = device_->activateJoysticks(joystickInfo_);
	} else {
		supportJoystick_ = false;
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
	joystickState_ = evt;
	return true;
}

void JoystickDevice::showInfo() const
{
	if(supportJoystick_) {
		std::cout << "Joystick support is enabled and " << joystickInfo_.size();
		std::cout<< "joystick(s) are present." << std::endl;

		for(size_t joystick = 0 ; joystick < joystickInfo_.size() ; ++joystick) {
			std::cout << "Joystick " << joystick << ":" << std::endl;
			std::cout << "\tName: " << joystickInfo_[joystick].Name.c_str() << std::endl;
			std::cout << "\tAxes: " << joystickInfo_[joystick].Axes << std::endl;
			std::cout << "\tButtons: " << joystickInfo_[joystick].Buttons << std::endl;
			std::cout << "\tHat is: ";

			switch(joystickInfo_[joystick].PovHat) {
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