// Ŭnicode please 
#include "stdafx.h"
#include "rock_paper_scissor.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"
#include "res.h"

using namespace std;
using namespace irr;

RockPaperScissor::RockPaperScissor(irr::scene::ICameraSceneNode *cam)
	: root_(nullptr)
{
	root_ = Lib::smgr->addEmptySceneNode(cam);
	root_->grab();
	root_->setRotation(core::vector3df(0, 180, 0));

	std::vector<irr::scene::SpriteSceneNode*> rpsSpriteList;
	std::vector<irr::scene::SpriteSceneNode*> padIconList;

	//생성한 sprite를 drop하는것은 list 루프에서 처리함
	{
		auto rockTex = Lib::driver->getTexture(res::texture::ICON_ROCK_PNG);
		rock_ = Lib::smgr->addEmptySceneNode(root_);
		rock_->setRotation(core::vector3df(0, -30, 0));
		auto sprite = new irr::scene::SpriteSceneNode(rock_, Lib::smgr, 0, rockTex);
		rpsSpriteList.push_back(sprite);
		
		auto xTex = Lib::driver->getTexture(res::texture::XBOX_PAD_X_PNG);
		auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, xTex);
		padIconList.push_back(icon);
	}
	{
		auto paperTex = Lib::driver->getTexture(res::texture::ICON_PAPER_PNG);
		scissor_ = Lib::smgr->addEmptySceneNode(root_);
		scissor_->setRotation(core::vector3df(-30, 0, 0));
		auto sprite = new irr::scene::SpriteSceneNode(scissor_, Lib::smgr, 0, paperTex);
		rpsSpriteList.push_back(sprite);

		auto iconTex = Lib::driver->getTexture(res::texture::XBOX_PAD_Y_PNG);
		auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, iconTex);
		padIconList.push_back(icon);
	}
	{
		auto scissorTex = Lib::driver->getTexture(res::texture::ICON_SCISSOR_PNG);
		paper_ = Lib::smgr->addEmptySceneNode(root_);
		paper_->setRotation(core::vector3df(0, 30, 0));
		auto sprite = new irr::scene::SpriteSceneNode(paper_, Lib::smgr, 0, scissorTex);
		rpsSpriteList.push_back(sprite);

		auto iconTex = Lib::driver->getTexture(res::texture::XBOX_PAD_B_PNG);
		auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, iconTex);
		padIconList.push_back(icon);
	}

	for(auto sprite : rpsSpriteList) {
		float radius = 30;
		core::vector3df spritePos(0, 0, radius);
		core::dimension2d<float> rpsSize(2, 2);
		sprite->setSize(rpsSize);
		sprite->setPosition(spritePos);
		sprite->drop();
	}

	for(auto icon : padIconList) {
		icon->setPosition(core::vector3df(0, -3, -0.5));
		icon->drop();
	}
}

RockPaperScissor::~RockPaperScissor()
{
	root_->drop();
	root_ = nullptr;
}

	
