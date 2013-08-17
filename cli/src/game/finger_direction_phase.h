// Ŭnicode please
#pragma once

namespace irr {;
namespace scene {;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class FingerDirectionEvent;
class FingerDirectionEventReceiver;
class FingerResult;

class BaseFingerDirectionPhase {
public:
	BaseFingerDirectionPhase(irr::scene::ICameraSceneNode *cam);
	virtual ~BaseFingerDirectionPhase();

	virtual void update(int ms) = 0;

	bool isEnd() const { return end_; }

	const FingerDirectionEvent &getAiChoice() const { return *aiChoice_; }
	const FingerDirectionEvent &getPlayerChoice() const;

	virtual std::unique_ptr<FingerResult> createResult(irr::scene::ICameraSceneNode *cam) = 0;

public:
	static std::unique_ptr<BaseFingerDirectionPhase> createAttack(irr::scene::ICameraSceneNode *cam);
	static std::unique_ptr<BaseFingerDirectionPhase> createDefense(irr::scene::ICameraSceneNode *cam);

protected:
	bool end_;
	
	irr::scene::ISceneNode *root_;
	irr::scene::Text3dSceneNode *centerText_;

	std::unique_ptr<FingerDirectionEvent> aiChoice_;

	FingerDirectionEventReceiver *evtReceiver_;
};