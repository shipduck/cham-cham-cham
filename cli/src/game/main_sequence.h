// Ŭnicode please 
#pragma once

#include "sequence.h"

class ScoreBoard;
class RockPaperScissor;
class AbstractHMDCameraEventReceiver;
class RockPaperScissorResult;
class BaseFingerDirectionPhase;
class FingerResult;

class SubSequence;

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
	irr::scene::ISceneNode *initSkybox();

private:
	AbstractHMDCameraEventReceiver *camReceiver_;
	irr::scene::IAnimatedMeshSceneNode *reinaNode_;
	std::unique_ptr<ScoreBoard> scoreBoard_;

	std::unique_ptr<SubSequence> subSequence_;

	/*
	std::unique_ptr<SubSequence> rps_;
	std::unique_ptr<RockPaperScissorResult> rpsResult_;
	std::unique_ptr<BaseFingerDirectionPhase> fingerPhase_;
	std::unique_ptr<FingerResult> fingerResult_;
	*/
};