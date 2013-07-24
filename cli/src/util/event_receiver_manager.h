// Ŭnicode please 
#pragma once

class JoystickDevice;
class EventReceiverManager;

extern EventReceiverManager *g_eventReceiverMgr;

struct HeadTrackingEvent {
	HeadTrackingEvent() : Yaw(0), Pitch(0), Roll(0) {}
	HeadTrackingEvent(float yaw, float pitch, float roll)
		: Yaw(yaw), Pitch(pitch), Roll(roll) {}
	float Yaw;
	float Pitch;
	float Roll;
};

// 게임 내부에서 사용하는 Event receiver이다
// HMD로 얻은 값까지 동일한 구조로 처리하기 위해서 도입함
class ICustomEventReceiver : public irr::IEventReceiver {
public:
	virtual ~ICustomEventReceiver() {}
	virtual bool OnEvent(const irr::SEvent &evt) = 0;
	virtual bool OnEvent(const HeadTrackingEvent &evt) = 0;
};

class EventReceiverManager : public ICustomEventReceiver {
public:
	struct PriorityEventReceiver {
		PriorityEventReceiver() : priority(0) {}
		PriorityEventReceiver(PriorityEventReceiver&& o) 
			: priority(o.priority), receiver(std::move(o.receiver)) { }
		PriorityEventReceiver &operator=(PriorityEventReceiver&& o) {
			this->priority = o.priority;
			this->receiver = std::move(o.receiver);
			return *this;
		}
		std::unique_ptr<ICustomEventReceiver> receiver;
		int priority;
	};

	typedef std::vector<PriorityEventReceiver> PriorityReceiverListType;

public:
	EventReceiverManager();
	~EventReceiverManager();

	void startUp(irr::IrrlichtDevice *dev);
	void shutDown();

	virtual bool OnEvent(const irr::SEvent &evt);
	bool OnEvent(const HeadTrackingEvent &evt);
	/*
	priority < 0은 high priority로 분리되서 디바이스 보다 먼저 처리됨
	priority >= 0은 low priority로 분리해서 디바이스 이후에 처리됨
	priority가 낮을수록 우선순위 높다
	*/
	ICustomEventReceiver *addReceiver(ICustomEventReceiver *receiver, int priority=0);

public:
	const JoystickDevice &getJoystickDev() const { return *joystickDev_; }

private:
	std::unique_ptr<JoystickDevice> joystickDev_;

	PriorityReceiverListType highPriorityReceiverList_;
	PriorityReceiverListType lowPriorityReceiverList_;
};

class JoystickDevice : public irr::IEventReceiver {
public:
	JoystickDevice();
	void setDevice(irr::IrrlichtDevice* device);

	virtual bool OnEvent(const irr::SEvent &evt);
	bool OnEvent(const irr::SEvent::SJoystickEvent &evt);

	void showInfo() const;

	const irr::core::array<irr::SJoystickInfo> &getJoystickInfo() const { return joystickInfo_; }

	template<int Axis>
	float getAxisFloatValue() const {
		int raw = getAxisIntValue<Axis>();
		return static_cast<float>(raw / 32767.0f);
	}

	template<int Axis>
	int getAxisIntValue() const {
		const auto &joystickData = joystickState_;
		return joystickData.Axis[Axis];
	}
private:
	irr::IrrlichtDevice *device_;
	irr::core::array<irr::SJoystickInfo> joystickInfo_;
	irr::SEvent::SJoystickEvent joystickState_;
	bool supportJoystick_;
};