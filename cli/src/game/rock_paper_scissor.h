// Ŭnicode please 
#pragma once

class RPSEvent {
public:
	typedef enum {
		kRock,
		kPaper,
		kScissor
	} value_type;

	value_type value;
};

class RockPaperScissor {
public:
	RockPaperScissor(irr::scene::ICameraSceneNode *cam);
	virtual ~RockPaperScissor();

private:
	irr::scene::ISceneNode *root_;

	irr::scene::ISceneNode *rock_;
	irr::scene::ISceneNode *paper_;
	irr::scene::ISceneNode *scissor_;
};