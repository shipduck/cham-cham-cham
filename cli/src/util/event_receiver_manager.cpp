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
	KeyboardDev.reset(new KeyboardDevice(dev));
	MouseDev.reset(new MouseDevice(dev));
}
void EventReceiverManager::shutDown()
{
	JoystickDev.reset(nullptr);
	KeyboardDev.reset(nullptr);
	MouseDev.reset(nullptr);

	LowPriorityReceiverList.clear();
	HighPriorityReceiverList.clear();
}

void EventReceiverManager::addReceiver(irr::IEventReceiver *receiver, int priority)
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

bool EventReceiverManager::OnEvent(const irr::SEvent &evt)
{
	for(auto &receiver : HighPriorityReceiverList) {
		receiver.Receiver->OnEvent(evt);
	}

	switch(evt.EventType) {
	case irr::EET_JOYSTICK_INPUT_EVENT:
		JoystickDev->OnEvent(evt.JoystickEvent);
		break;
	case irr::EET_KEY_INPUT_EVENT:
		KeyboardDev->OnEvent(evt.KeyInput);
		break;
	case irr::EET_MOUSE_INPUT_EVENT:
		MouseDev->OnEvent(evt.MouseInput);
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

KeyboardDevice::KeyboardDevice(irr::IrrlichtDevice *dev)
{
}

bool KeyboardDevice::OnEvent(const irr::SEvent &evt)
{
	if(evt.EventType != irr::EET_KEY_INPUT_EVENT) {
		return false;
	}
	return OnEvent(evt.KeyInput);
}
bool KeyboardDevice::OnEvent(const irr::SEvent::SKeyInput &evt)
{
	return true;
}

MouseDevice::MouseDevice(irr::IrrlichtDevice *dev)
{
}

bool MouseDevice::OnEvent(const irr::SEvent &evt)
{
	if(evt.EventType != irr::EET_MOUSE_INPUT_EVENT) {
		return false;
	}
	return OnEvent(evt.MouseInput);
}

bool MouseDevice::OnEvent(const irr::SEvent::SMouseInput &evt)
{
	switch(evt.Event) {
	case EMIE_LMOUSE_PRESSED_DOWN:
		LeftDown = true;
		break;
	case EMIE_LMOUSE_LEFT_UP:
		LeftDown = false;
		break;
	case EMIE_RMOUSE_PRESSED_DOWN:
		RightDown = true;
		break;
	case EMIE_RMOUSE_LEFT_UP:
		RightDown = false;
		break;
	case EMIE_MOUSE_MOVED:
		Position.Y = evt.Y;
		Position.Y = evt.Y;
		break;

	default:
		break;
	}
	return true;
}