// Ŭnicode please 
#pragma once

class JoystickInputEvent;
class KeyboardDevice;
class MouseDevice;
class EventReceiverManager;

extern EventReceiverManager *gEventReceiverMgr;

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
		PriorityEventReceiver() : Priority(0) {}
		PriorityEventReceiver(PriorityEventReceiver&& o) 
			: Priority(o.Priority), Receiver(std::move(o.Receiver)) { }
		PriorityEventReceiver &operator=(PriorityEventReceiver&& o) {
			this->Priority = o.Priority;
			this->Receiver = std::move(o.Receiver);
			return *this;
		}
		std::unique_ptr<ICustomEventReceiver> Receiver;
		int Priority;
	};

	struct PriorityCustomEventReceiver {
		ICustomEventReceiver *Receiver;
		int Priority;
	};
	
	typedef std::vector<PriorityEventReceiver> PriorityReceiverListType;

public:
	EventReceiverManager();
	~EventReceiverManager();

	void setUp(irr::IrrlichtDevice *dev);
	void shutDown();
	void update(int ms);

	virtual bool OnEvent(const irr::SEvent &evt);
	bool OnEvent(const HeadTrackingEvent &evt);
	/*
	priority < 0은 high priority로 분리되서 디바이스 보다 먼저 처리됨
	priority >= 0은 low priority로 분리해서 디바이스 이후에 처리됨
	priority가 낮을수록 우선순위 높다
	*/
	void addReceiver(ICustomEventReceiver *receiver, int priority=0);
public:
	const JoystickInputEvent &getJoystickDev() const { return *JoystickDev; }

private:
	std::unique_ptr<JoystickInputEvent> JoystickDev;

	PriorityReceiverListType HighPriorityReceiverList;
	PriorityReceiverListType LowPriorityReceiverList;
};