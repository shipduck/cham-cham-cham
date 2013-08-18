// Ŭnicode please 
#pragma once

#include "finger_direction_phase.h"

class DefensePhase : public BaseFingerDirectionPhase {
public:
	DefensePhase(irr::scene::ICameraSceneNode *cam, ScoreBoard *board);
	virtual ~DefensePhase();
	virtual std::unique_ptr<SubSequence> update(int ms);
	virtual SubSequence *createResult();
};