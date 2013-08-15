// Ŭnicode please 
#pragma once

#include "sequence.h"

class ScoreBoard;
class AbstractHMDCameraEventReceiver;

enum {
	kStateRockScissorPaper,
	kStateChamChamCham,
	kStateGameWin,
	kStateGameLose,
};

class MainSequence : public Sequence {
public:
	MainSequence();
	virtual ~MainSequence();

	virtual void update(int ms);
	virtual void preDraw() {}
	virtual void postDraw() {}

private:
	AbstractHMDCameraEventReceiver *receiver_;
	irr::scene::IAnimatedMeshSceneNode *reinaNode_;
	std::unique_ptr<ScoreBoard> scoreBoard_;
};