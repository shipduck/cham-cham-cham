// Ŭnicode please 
#include "stdafx.h"
#include "res.h"
#include "chamchamcham.h"
#include "irr/text_3d_scene_node.h"
#include "irr/sprite_scene_node.h"
#include "base/lib.h"
#include "irr/debug_drawer.h"
#include "util/event_receiver_manager.h"

using namespace std;
using namespace irr;

bool ChamChamChamEvent::operator==(const ChamChamChamEvent &o) const
{
	return (this->value == o.value);
}
bool ChamChamChamEvent::operator!=(const ChamChamChamEvent &o) const
{
	return !(*this == o);
}

///////////////////////////////////////

BaseChamChamCham::BaseChamChamCham(irr::scene::ICameraSceneNode *cam)
	: end_(false),
	root_(nullptr),
	centerText_(nullptr)
{
	root_ = Lib::smgr->addEmptySceneNode(cam);
	root_->grab();
	root_->setRotation(core::vector3df(0, 180, 0));

	centerText_ = new irr::scene::Text3dSceneNode(root_, Lib::smgr, 0, getDebugFont(), L"?"); 
	centerText_->setPosition(core::vector3df(0, 0, 30.0f));
	auto size = core::dimension2d<f32>(5.0f, 5.0f);
	centerText_->setSize(size);
	centerText_->drop();

	std::vector<irr::scene::SpriteSceneNode*> arrowList;
	{
		auto leftTex = Lib::driver->getTexture(res::texture::ARROW_HOLLOW_LEFT_PNG);
		auto node = Lib::smgr->addEmptySceneNode(root_);
		node->setRotation(core::vector3df(0, -20, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, leftTex);
		arrowList.push_back(sprite);
		
		auto xTex = Lib::driver->getTexture(res::texture::XBOX_PAD_X_PNG);
		auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, xTex);
		icon->setPosition(core::vector3df(0, 0, -0.5));
		icon->drop();
	}
	{
		auto upTex = Lib::driver->getTexture(res::texture::ARROW_HOLLOW_UP_PNG);
		auto node = Lib::smgr->addEmptySceneNode(root_);
		node->setRotation(core::vector3df(-20, 0, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, upTex);
		arrowList.push_back(sprite);
		
		auto yTex = Lib::driver->getTexture(res::texture::XBOX_PAD_Y_PNG);
		auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, yTex);
		icon->setPosition(core::vector3df(0, 0, -0.5));
		icon->drop();
	}
	{
		auto rightTex = Lib::driver->getTexture(res::texture::ARROW_HOLLOW_RIGHT_PNG);
		auto node = Lib::smgr->addEmptySceneNode(root_);
		node->setRotation(core::vector3df(0, 20, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, rightTex);
		arrowList.push_back(sprite);
		
		auto bTex = Lib::driver->getTexture(res::texture::XBOX_PAD_B_PNG);
		auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, bTex);
		icon->setPosition(core::vector3df(0, 0, -0.5));
		icon->drop();
	}
	{
		auto downTex = Lib::driver->getTexture(res::texture::ARROW_HOLLOW_DOWN_PNG);
		auto node = Lib::smgr->addEmptySceneNode(root_);
		node->setRotation(core::vector3df(20, 0, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, downTex);
		arrowList.push_back(sprite);
		
		auto aTex = Lib::driver->getTexture(res::texture::XBOX_PAD_A_PNG);
		auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, aTex);
		icon->setPosition(core::vector3df(0, 0, -0.5));
		icon->drop();
	}

	for(auto sprite : arrowList) {
		float radius = 30;
		core::vector3df spritePos(0, 0, radius);
		core::dimension2d<float> rpsSize(2, 2);
		sprite->setSize(rpsSize);
		sprite->setPosition(spritePos);
		sprite->drop();
	}
}

BaseChamChamCham::~BaseChamChamCham()
{
	root_->remove();
	root_->drop();
	root_ = nullptr;
}

///////////////////////////////////////
ChamChamChamAttack::ChamChamChamAttack(irr::scene::ICameraSceneNode *cam)
	: BaseChamChamCham(cam)
{
	centerText_->setText(L"Attack");
}
void ChamChamChamAttack::update(int ms)
{
}

///////////////////////////////////////

ChamChamChamDefense::ChamChamChamDefense(irr::scene::ICameraSceneNode *cam)
	: BaseChamChamCham(cam)
{
	centerText_->setText(L"Defense");
}
void ChamChamChamDefense::update(int ms)
{
}

