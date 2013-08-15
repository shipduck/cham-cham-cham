// Ŭnicode please 
#pragma once

class RockScissorPaper {
public:
	RockScissorPaper(irr::scene::ICameraSceneNode *cam);
	virtual ~RockScissorPaper();

private:
	irr::scene::ISceneNode *root_;

	irr::scene::ISceneNode *rock_;
	irr::scene::ISceneNode *scissor_;
	irr::scene::ISceneNode *paper_;
};