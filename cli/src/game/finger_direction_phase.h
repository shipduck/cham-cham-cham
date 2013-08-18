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

class BaseFingerDirectionPhase : public SubSequence {
public:
	BaseFingerDirectionPhase(irr::scene::ICameraSceneNode *cam, ScoreBoard *board);
	virtual ~BaseFingerDirectionPhase();

	virtual std::unique_ptr<SubSequence> update(int ms) = 0;

	const FingerDirectionEvent &getAiChoice() const { return *aiChoice_; }
	const FingerDirectionEvent &getPlayerChoice() const;

	virtual SubSequence *createResult() = 0;

protected:
	irr::scene::ISceneNode *root_;
	irr::scene::Text3dSceneNode *centerText_;

	std::unique_ptr<FingerDirectionEvent> aiChoice_;

	FingerDirectionEventReceiver *evtReceiver_;

	int effect_;
};