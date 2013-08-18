// Ŭnicode please 
#pragma once

#include "sub_sequence.h"


class RPSEvent;
class RPSEventReceiver;

class RockPaperScissor : public SubSequence {
public:
	RockPaperScissor(irr::scene::ICameraSceneNode *cam, ScoreBoard *board);
	virtual ~RockPaperScissor();

	std::unique_ptr<SubSequence> update(int ms);

	const RPSEvent &getAiChoice() const { return *aiChoice_; }
	const RPSEvent &getPlayerChoice() const { return *playerChoice_; }

private:
	void attachEventReceiver();
	void detachEventReceiver();

private:
	irr::scene::ISceneNode *root_;

	irr::scene::ISceneNode *rock_;
	irr::scene::ISceneNode *paper_;
	irr::scene::ISceneNode *scissor_;

	RPSEventReceiver *evtReceiver_;

private:
	// 가위바위보 결과를 임시 저장
	std::unique_ptr<RPSEvent> aiChoice_;
	std::unique_ptr<RPSEvent> playerChoice_;

	int effect_;
};
