// Ŭnicode please 
#pragma once

class JoystickDevice;
class EventReceiverManager;
struct SHeadTrackingEvent;
struct SLeapMotionEvent;

// 게임 내부에서 사용하는 Event receiver이다
// HMD로 얻은 값까지 동일한 구조로 처리하기 위해서 도입함
class ICustomEventReceiver : public irr::IEventReceiver {
public:
	ICustomEventReceiver() {}
	virtual ~ICustomEventReceiver() {}
	virtual bool OnEvent(const irr::SEvent &evt) = 0;
	virtual bool OnEvent(const SHeadTrackingEvent &evt) = 0;
#ifdef USE_LEAP_MOTION
	virtual bool OnEvent(const SLeapMotionEvent &evt) { return false;}
#endif
};

class EventReceiverManager : public ICustomEventReceiver {
public:
	struct SPriorityEventReceiver {
		SPriorityEventReceiver() : priority(0) {}
		SPriorityEventReceiver(SPriorityEventReceiver && o) 
			: priority(o.priority), receiver(std::move(o.receiver)) { }
		SPriorityEventReceiver &operator=(SPriorityEventReceiver&& o) {
			this->priority = o.priority;
			this->receiver = std::move(o.receiver);
			return *this;
		}
		std::unique_ptr<ICustomEventReceiver> receiver;
		int priority;
        bool operator<(const SPriorityEventReceiver& other) const {
            return priority < other.priority;
        }
	};

	typedef std::multiset<SPriorityEventReceiver> PriorityReceiverListType;

public:
	EventReceiverManager();
	~EventReceiverManager();

	void startUp(irr::IrrlichtDevice *dev);
	void shutDown();

	virtual bool OnEvent(const irr::SEvent &evt);
	bool OnEvent(const SHeadTrackingEvent &evt);
#ifdef USE_LEAP_MOTION
	bool OnEvent(const SLeapMotionEvent &evt);
#endif
	/*
	priority < 0은 high priority로 분리되서 디바이스 보다 먼저 처리됨
	priority >= 0은 low priority로 분리해서 디바이스 이후에 처리됨
	priority가 낮을수록 우선순위 높다
	*/
	ICustomEventReceiver *attachReceiver(ICustomEventReceiver *receiver, int priority=0);
    bool detachReceiver(ICustomEventReceiver *receiver);

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