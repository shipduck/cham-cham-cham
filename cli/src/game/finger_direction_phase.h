// Ŭnicode please
#pragma once

#include "sub_sequence.h"

namespace irr {;
namespace scene {;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class FingerDirectionEvent;
class FingerDirectionEventReceiver;
class FingerResult;

class BaseFingerDirectionPhase : public SubSequence {
public:
	BaseFingerDirectionPhase(irr::scene::ICameraSceneNode *cam);
	virtual ~BaseFingerDirectionPhase();

	virtual std::unique_ptr<SubSequence> update(int ms) = 0;

	bool isEnd() const { return end_; }

	const FingerDirectionEvent &getAiChoice() const { return *aiChoice_; }
	const FingerDirectionEvent &getPlayerChoice() const;

	virtual std::unique_ptr<FingerResult> createResult(irr::scene::ICameraSceneNode *cam) = 0;

public:
	static BaseFingerDirectionPhase *createAttackRawPtr(irr::scene::ICameraSceneNode *cam);
	static BaseFingerDirectionPhase *createDefenseRawPtr(irr::scene::ICameraSceneNode *cam);
	static std::unique_ptr<BaseFingerDirectionPhase> createAttack(irr::scene::ICameraSceneNode *cam);
	static std::unique_ptr<BaseFingerDirectionPhase> createDefense(irr::scene::ICameraSceneNode *cam);

protected:
	bool end_;
	
	irr::scene::ISceneNode *root_;
	irr::scene::Text3dSceneNode *centerText_;

	std::unique_ptr<FingerDirectionEvent> aiChoice_;

	FingerDirectionEventReceiver *evtReceiver_;
};