// Ŭnicode please 
#pragma once

#include "finger_direction_phase.h"

class AttackPhase : public BaseFingerDirectionPhase {
public:
	AttackPhase(irr::scene::ICameraSceneNode *cam, ScoreBoard *board);
	virtual ~AttackPhase();
	virtual std::unique_ptr<SubSequence> update(int ms);
	virtual SubSequence *createResult();
};
