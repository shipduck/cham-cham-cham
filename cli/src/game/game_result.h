// Ŭnicode please 
#pragma once

#include "sub_sequence.h"

class RPSEvent;
class FingerDirectionEvent;

namespace irr {;
namespace scene {;
class SpriteSceneNode;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class AbstractGameResult : public SubSequence {
public:
	AbstractGameResult(irr::scene::ICameraSceneNode *cam, ScoreBoard *board, int remain);
	virtual ~AbstractGameResult();

public:
	irr::scene::ISceneNode *getRoot() { return root_; }

	void setAiTexture(irr::video::ITexture *tex);
	void setPlayerTexture(irr::video::ITexture *tex);
	void setText(const wchar_t *txt);

	virtual std::unique_ptr<SubSequence> update(int ms);
	virtual SubSequence *createNext() = 0;

private:
	irr::scene::SpriteSceneNode *getAiNode() { return aiNode_; }
	irr::scene::SpriteSceneNode *getPlayerNode() { return playerNode_; }
	irr::scene::Text3dSceneNode *getTextNode() { return textNode_; }

private:
	irr::scene::ISceneNode *root_;
	irr::scene::SpriteSceneNode *aiNode_;
	irr::scene::SpriteSceneNode *playerNode_;
	irr::scene::Text3dSceneNode *textNode_;

	int remain_;
};

class RockPaperScissorResult : public AbstractGameResult {
public:
	RockPaperScissorResult(irr::scene::ICameraSceneNode *cam,
		ScoreBoard *board, 
		const RPSEvent &playerChoice,
		const RPSEvent &aiChoice);
	virtual ~RockPaperScissorResult();

	const RPSEvent &getAiChoice() const { return *aiChoice_; }
	const RPSEvent &getPlayerChoice() const { return *playerChoice_; }

	virtual SubSequence *createNext();

private:
	std::unique_ptr<RPSEvent> aiChoice_;
	std::unique_ptr<RPSEvent> playerChoice_;

	irr::video::ITexture *getRPSTexture(int rps);
};

class FingerResult : public AbstractGameResult {
public:
	FingerResult(irr::scene::ICameraSceneNode *cam,
		ScoreBoard *board,
		const FingerDirectionEvent &playerChoice,
		const FingerDirectionEvent &aiChoice);
	virtual ~FingerResult();

	virtual SubSequence *createNext();

public:
	const FingerDirectionEvent &getAiChoice() const { return *aiChoice_; }
	const FingerDirectionEvent &getPlayerChoice() const { return *playerChoice_; }

private:
	std::unique_ptr<FingerDirectionEvent> aiChoice_;
	std::unique_ptr<FingerDirectionEvent> playerChoice_;
};

class AttackResult : public FingerResult {
public:
	AttackResult(irr::scene::ICameraSceneNode *cam,
		ScoreBoard *board,
		const FingerDirectionEvent &playerChoice,
		const FingerDirectionEvent &aiChoice);
	virtual ~AttackResult() {}
};

class DefenseResult : public FingerResult {
public:
	DefenseResult(irr::scene::ICameraSceneNode *cam,
		ScoreBoard *board, 
		const FingerDirectionEvent &playerChoice,
		const FingerDirectionEvent &aiChoice);
	virtual ~DefenseResult() {}
};