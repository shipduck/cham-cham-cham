// Ŭnicode please
#pragma once

namespace irr {;
namespace scene {;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class FingerDirectionEvent;
class FingerDirectionEventReceiver;

class BaseFingerDirectionPhase {
public:
	BaseFingerDirectionPhase(irr::scene::ICameraSceneNode *cam);
	virtual ~BaseFingerDirectionPhase();

	virtual void update(int ms) = 0;

	bool isEnd() const { return end_; }

	const FingerDirectionEvent &getAiChoice() const { return *aiChoice_; }
	const FingerDirectionEvent &getPlayerChoice() const;

protected:
	bool end_;
	
	irr::scene::ISceneNode *root_;
	irr::scene::Text3dSceneNode *centerText_;

	FingerDirectionEvent choiceAIEvent() const;
	std::unique_ptr<FingerDirectionEvent> aiChoice_;

	FingerDirectionEventReceiver *evtReceiver_;
};

class AttackPhase : public BaseFingerDirectionPhase {
public:
	AttackPhase(irr::scene::ICameraSceneNode *cam);
	virtual ~AttackPhase();

	void update(int ms);

};

class DefensePhase : public BaseFingerDirectionPhase {
public:
	DefensePhase(irr::scene::ICameraSceneNode *cam);
	virtual ~DefensePhase();

	void update(int ms);
private:
	
};