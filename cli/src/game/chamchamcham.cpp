// Ŭnicode please 
#include "stdafx.h"
#include "res.h"
#include "chamchamcham.h"
#include "irr/text_3d_scene_node.h"
#include "irr/sprite_scene_node.h"
#include "base/lib.h"
#include "irr/debug_drawer.h"
#include "util/event_receiver_manager.h"
#include "finger_direction_event.h"

using namespace std;
using namespace irr;


BaseChamChamCham::BaseChamChamCham(irr::scene::ICameraSceneNode *cam)
	: end_(false),
	root_(nullptr),
	centerText_(nullptr),
	evtReceiver_(nullptr)
{
	root_ = Lib::smgr->addEmptySceneNode(cam);
	root_->grab();
	root_->setRotation(core::vector3df(0, 180, 0));

	centerText_ = new irr::scene::Text3dSceneNode(root_, Lib::smgr, 0, getDebugFont(), L"?"); 
	centerText_->setPosition(core::vector3df(0, 0, 30.0f));
	auto size = core::dimension2d<f32>(5.0f, 5.0f);
	centerText_->setSize(size);
	centerText_->drop();

	const bool usePadIcon = false;

	std::vector<irr::scene::SpriteSceneNode*> arrowList;
	{
		auto leftTex = Lib::driver->getTexture(res::texture::ARROW_HOLLOW_LEFT_PNG);
		auto node = Lib::smgr->addEmptySceneNode(root_);
		node->setRotation(core::vector3df(0, -20, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, leftTex);
		arrowList.push_back(sprite);
		
		if(usePadIcon) {
			auto xTex = Lib::driver->getTexture(res::texture::XBOX_PAD_X_PNG);
			auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, xTex);
			icon->setPosition(core::vector3df(0, 0, -0.5));
			icon->drop();
		}
	}
	if(FingerDirectionEvent::getDirectionCount() > 2) {
		auto upTex = Lib::driver->getTexture(res::texture::ARROW_HOLLOW_UP_PNG);
		auto node = Lib::smgr->addEmptySceneNode(root_);
		node->setRotation(core::vector3df(-20, 0, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, upTex);
		arrowList.push_back(sprite);
		
		if(usePadIcon) {
			auto yTex = Lib::driver->getTexture(res::texture::XBOX_PAD_Y_PNG);
			auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, yTex);
			icon->setPosition(core::vector3df(0, 0, -0.5));
			icon->drop();
		}
	}
	{
		auto rightTex = Lib::driver->getTexture(res::texture::ARROW_HOLLOW_RIGHT_PNG);
		auto node = Lib::smgr->addEmptySceneNode(root_);
		node->setRotation(core::vector3df(0, 20, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, rightTex);
		arrowList.push_back(sprite);
		
		if(usePadIcon) {
			auto bTex = Lib::driver->getTexture(res::texture::XBOX_PAD_B_PNG);
			auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, bTex);
			icon->setPosition(core::vector3df(0, 0, -0.5));
			icon->drop();
		}
	}
	if(FingerDirectionEvent::getDirectionCount() > 2) {
		auto downTex = Lib::driver->getTexture(res::texture::ARROW_HOLLOW_DOWN_PNG);
		auto node = Lib::smgr->addEmptySceneNode(root_);
		node->setRotation(core::vector3df(20, 0, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, downTex);
		arrowList.push_back(sprite);
		
		if(usePadIcon) {
			auto aTex = Lib::driver->getTexture(res::texture::XBOX_PAD_A_PNG);
			auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, aTex);
			icon->setPosition(core::vector3df(0, 0, -0.5));
			icon->drop();
		}
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
	if(evtReceiver_ != nullptr) {
		Lib::eventReceiver->detachReceiver(evtReceiver_);
		evtReceiver_ = nullptr;
	}

	root_->remove();
	root_->drop();
	root_ = nullptr;
}

FingerDirectionEvent BaseChamChamCham::choiceAIEvent() const
{
	static std::default_random_engine e1(time(nullptr));
	std::uniform_int_distribution<int> randGen(1, FingerDirectionEvent::getDirectionCount());
	auto value = randGen(e1);
	return FingerDirectionEvent(value);
}

const FingerDirectionEvent &BaseChamChamCham::getPlayerChoice() const
{
	return evtReceiver_->inputEvt;
}


///////////////////////////////////////

ChamChamChamAttack::ChamChamChamAttack(irr::scene::ICameraSceneNode *cam)
	: BaseChamChamCham(cam)
{
	centerText_->setText(L"Attack");

	evtReceiver_ = new FingerDirectionEventReceiver(true, false, true);
	Lib::eventReceiver->attachReceiver(evtReceiver_);
}
ChamChamChamAttack::~ChamChamChamAttack()
{
	
}
void ChamChamChamAttack::update(int ms)
{
	if(isEnd() == true) {
		return;
	}
	if(evtReceiver_->inputEvt.isValid() == false) {
		return;
	}

	aiChoice_.reset(new FingerDirectionEvent(this->choiceAIEvent()));
	end_ = true;
}

///////////////////////////////////////

ChamChamChamDefense::ChamChamChamDefense(irr::scene::ICameraSceneNode *cam)
	: BaseChamChamCham(cam)
{
	centerText_->setText(L"Defense");

	evtReceiver_ = new FingerDirectionEventReceiver(true, true, false);
	Lib::eventReceiver->attachReceiver(evtReceiver_);
}

ChamChamChamDefense::~ChamChamChamDefense()
{
}

void ChamChamChamDefense::update(int ms)
{
	if(isEnd() == true) {
		return;
	}
	if(evtReceiver_->inputEvt.isValid() == false) {
		return;
	}

	aiChoice_.reset(new FingerDirectionEvent(this->choiceAIEvent()));
	end_ = true;
}
