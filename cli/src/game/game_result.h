// Ŭnicode please 
#pragma once

class RPSEvent;
class FingerDirectionEvent;

namespace irr {;
namespace scene {;
class SpriteSceneNode;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class AbstractGameResult {
public:
	AbstractGameResult(irr::scene::ICameraSceneNode *cam);
	virtual ~AbstractGameResult();

public:
	irr::scene::ISceneNode *getRoot() { return root_; }

	void setAiTexture(irr::video::ITexture *tex);
	void setPlayerTexture(irr::video::ITexture *tex);
	void setText(const wchar_t *txt);

private:
	irr::scene::SpriteSceneNode *getAiNode() { return aiNode_; }
	irr::scene::SpriteSceneNode *getPlayerNode() { return playerNode_; }
	irr::scene::Text3dSceneNode *getTextNode() { return textNode_; }

private:
	irr::scene::ISceneNode *root_;
	irr::scene::SpriteSceneNode *aiNode_;
	irr::scene::SpriteSceneNode *playerNode_;
	irr::scene::Text3dSceneNode *textNode_;
};

class RockPaperScissorResult : public AbstractGameResult {
public:
	RockPaperScissorResult(irr::scene::ICameraSceneNode *cam,
		const RPSEvent &playerChoice,
		const RPSEvent &aiChoice);
	virtual ~RockPaperScissorResult();

	const RPSEvent &getAiChoice() const { return *aiChoice_; }
	const RPSEvent &getPlayerChoice() const { return *playerChoice_; }

private:
	std::unique_ptr<RPSEvent> aiChoice_;
	std::unique_ptr<RPSEvent> playerChoice_;

	irr::video::ITexture *getRPSTexture(int rps);
};

class AttackResult : public AbstractGameResult {
public:
	AttackResult(irr::scene::ICameraSceneNode *cam,
		const FingerDirectionEvent &playerChoice,
		const FingerDirectionEvent &aiChoice);
	virtual ~AttackResult();

	const FingerDirectionEvent &getAiChoice() const { return *aiChoice_; }
	const FingerDirectionEvent &getPlayerChoice() const { return *playerChoice_; }

private:
	std::unique_ptr<FingerDirectionEvent> aiChoice_;
	std::unique_ptr<FingerDirectionEvent> playerChoice_;
};

class DefenseResult : public AbstractGameResult {
public:
	DefenseResult(irr::scene::ICameraSceneNode *cam,
		const FingerDirectionEvent &playerChoice,
		const FingerDirectionEvent &aiChoice);
	virtual ~DefenseResult();

	const FingerDirectionEvent &getAiChoice() const { return *aiChoice_; }
	const FingerDirectionEvent &getPlayerChoice() const { return *playerChoice_; }
private:
	std::unique_ptr<FingerDirectionEvent> aiChoice_;
	std::unique_ptr<FingerDirectionEvent> playerChoice_;
};