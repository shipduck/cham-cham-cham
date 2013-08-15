// Ŭnicode please 
#include "stdafx.h"
#include "rock_scissor_paper.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"
#include "res.h"

using namespace std;
using namespace irr;

RockScissorPaper::RockScissorPaper(irr::scene::ICameraSceneNode *cam)
	: root_(nullptr)
{
	root_ = Lib::smgr->addEmptySceneNode(cam);
	root_->grab();
	root_->setRotation(core::vector3df(0, 180, 0));

	auto rockTex = Lib::driver->getTexture(res::texture::ICON_ROCK_PNG);
	auto scissorTex = Lib::driver->getTexture(res::texture::ICON_SCISSOR_PNG);
	auto paperTex = Lib::driver->getTexture(res::texture::ICON_PAPER_PNG);
	float radius = 30;
	core::vector3df spritePos(0, 0, radius);
	{
		rock_ = Lib::smgr->addEmptySceneNode(root_);
		rock_->setRotation(core::vector3df(0, -30, 0));
		auto sprite = new irr::scene::SpriteSceneNode(rock_, Lib::smgr, 0, rockTex);
		sprite->setSize(core::dimension2d<float>(2, 2));
		sprite->setPosition(spritePos);
		sprite->drop();
	}
	{
		scissor_ = Lib::smgr->addEmptySceneNode(root_);
		scissor_->setRotation(core::vector3df(-30, 0, 0));
		auto sprite = new irr::scene::SpriteSceneNode(scissor_, Lib::smgr, 0, scissorTex);
		sprite->setSize(core::dimension2d<float>(2, 2));
		sprite->setPosition(spritePos);
		sprite->drop();
	}
	{
		paper_ = Lib::smgr->addEmptySceneNode(root_);
		paper_->setRotation(core::vector3df(0, 30, 0));
		auto sprite = new irr::scene::SpriteSceneNode(paper_, Lib::smgr, 0, paperTex);
		sprite->setSize(core::dimension2d<float>(2, 2));
		sprite->setPosition(spritePos);
		sprite->drop();
	}
}

RockScissorPaper::~RockScissorPaper()
{
	root_->drop();
	root_ = nullptr;
}

	
