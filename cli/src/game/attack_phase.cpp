// Ŭnicode please 
#include "stdafx.h"
#include "attack_phase.h"
#include "ai_player.h"
#include "base/lib.h"
#include "game_result.h"
#include "irr/text_3d_scene_node.h"
#include "finger_direction_event.h"

using namespace std;
using namespace irr;


AttackPhase::AttackPhase(irr::scene::ICameraSceneNode *cam, ScoreBoard *board)
	: BaseFingerDirectionPhase(cam, board)
{
	centerText_->setText(L"Attack");

	evtReceiver_ = new FingerDirectionEventReceiver(true, false, true);
	Lib::eventReceiver->attachReceiver(evtReceiver_);
}
AttackPhase::~AttackPhase()
{
	
}

std::unique_ptr<SubSequence> AttackPhase::update(int ms)
{
	std::unique_ptr<SubSequence> empty;
	if(evtReceiver_->inputEvt.isValid() == false) {
		return empty;
	}
	FingerDirectionEvent aiEvt = AiPlayer().think(evtReceiver_->inputEvt);
	aiChoice_.reset(new FingerDirectionEvent(aiEvt));

	return std::unique_ptr<SubSequence>(createResult());
}

SubSequence *AttackPhase::createResult()
{
	auto ai = this->getAiChoice();
	auto player = this->getPlayerChoice();
	return new AttackResult(getCamera(), getScoreBoard(), player, ai);
}