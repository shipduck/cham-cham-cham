// Ŭnicode please 
#pragma once

class JoystickDevice;
class KeyboardDevice;
class MouseDevice;
class EventReceiverManager;

extern EventReceiverManager *gEventReceiverMgr;

class EventReceiverManager : public irr::IEventReceiver {
public:
	struct PriorityEventReceiver {
		PriorityEventReceiver() : Priority(0) {}
		PriorityEventReceiver(PriorityEventReceiver&& o) 
			: Priority(o.Priority), Receiver(std::move(o.Receiver)) { }
		PriorityEventReceiver &operator=(PriorityEventReceiver&& o) {
			this->Priority = o.Priority;
			this->Receiver = std::move(o.Receiver);
			return *this;
		}
		std::unique_ptr<irr::IEventReceiver> Receiver;
		int Priority;
	};
	
	typedef std::vector<PriorityEventReceiver> PriorityReceiverListType;

public:
	EventReceiverManager();
	~EventReceiverManager();

	void setUp(irr::IrrlichtDevice *dev);
	void shutDown();

	virtual bool OnEvent(const irr::SEvent &evt);
	/*
	priority < 0은 high priority로 분리되서 디바이스 보다 먼저 처리됨
	priority >= 0은 low priority로 분리해서 디바이스 이후에 처리됨
	priority가 낮을수록 우선순위 높다
	*/
	void addReceiver(irr::IEventReceiver *receiver, int priority=0);
public:
	const JoystickDevice &getJoystickDev() const { return *JoystickDev; }
	const KeyboardDevice &getKeyboardDev() const { return *KeyboardDev; }
	const MouseDevice &getMouseDev() const { return *MouseDev; }

private:
	std::unique_ptr<JoystickDevice> JoystickDev;
	std::unique_ptr<KeyboardDevice> KeyboardDev;
	std::unique_ptr<MouseDevice> MouseDev;

	PriorityReceiverListType HighPriorityReceiverList;
	PriorityReceiverListType LowPriorityReceiverList;
};

class JoystickDevice : public irr::IEventReceiver {
public:
	JoystickDevice(irr::IrrlichtDevice *dev);
	virtual bool OnEvent(const irr::SEvent &evt);
	bool OnEvent(const irr::SEvent::SJoystickEvent &evt);

	void showInfo() const;

	const irr::SJoystickInfo &getJoystickInfo() const { return JoystickInfo[0]; }

	template<int Axis>
	float getAxisFloatValue() const {
		int raw = getAxisIntValue<Axis>();
		return static_cast<float>(raw / 32767.0f);
	}

	template<int Axis>
	int getAxisIntValue() const {
		const auto &joystickData = JoystickState;
		return joystickData.Axis[Axis];
	}
private:
	irr::IrrlichtDevice *Device;
	irr::core::array<irr::SJoystickInfo> JoystickInfo;
	irr::SEvent::SJoystickEvent JoystickState;
	bool SupportJoystick;
};

class KeyboardDevice : public irr::IEventReceiver {
public:
	KeyboardDevice(irr::IrrlichtDevice *dev);
	virtual bool OnEvent(const irr::SEvent &evt);
	bool OnEvent(const irr::SEvent::SKeyInput &evt);
};

class MouseDevice : public irr::IEventReceiver {
public:
	MouseDevice(irr::IrrlichtDevice *dev);
	virtual bool OnEvent(const irr::SEvent &evt);
	bool OnEvent(const irr::SEvent::SMouseInput &evt);

	const bool isLeftDown() const { return LeftDown; }
	const bool isRightDown() const { return RightDown; }
	const irr::core::position2di &getPosition() const { return Position; }

public:
	bool LeftDown;
	bool RightDown;
	irr::core::position2di Position;
};
