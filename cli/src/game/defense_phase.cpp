// Ŭnicode please 
#include "stdafx.h"
#include "defense_phase.h"
#include "base/lib.h"
#include "game_result.h"
#include "finger_direction_event.h"
#include "irr/text_3d_scene_node.h"
#include "ai_player.h"

using namespace std;
using namespace irr;


DefensePhase::DefensePhase(irr::scene::ICameraSceneNode *cam, ScoreBoard *board)
	: BaseFingerDirectionPhase(cam, board)
{
	centerText_->setText(L"Defense");

	evtReceiver_ = new FingerDirectionEventReceiver(true, true, false);
	Lib::eventReceiver->attachReceiver(evtReceiver_);
}

DefensePhase::~DefensePhase()
{
}

std::unique_ptr<SubSequence> DefensePhase::update(int ms)
{
	std::unique_ptr<SubSequence> empty;
	if(evtReceiver_->inputEvt.isValid() == false) {
		return empty;
	}

	FingerDirectionEvent aiEvt = AiPlayer().think(evtReceiver_->inputEvt);
	aiChoice_.reset(new FingerDirectionEvent(aiEvt));
	
	return std::unique_ptr<SubSequence>(createResult());
}

SubSequence *DefensePhase::createResult()
{
	auto ai = this->getAiChoice();
	auto player = this->getPlayerChoice();
	return new DefenseResult(getCamera(), getScoreBoard(), player, ai);
}