// Ŭnicode please 
#pragma once

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