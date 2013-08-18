// Ŭnicode please 
#include "stdafx.h"
#include "res.h"
#include "finger_direction_phase.h"
#include "irr/text_3d_scene_node.h"
#include "irr/sprite_scene_node.h"
#include "base/lib.h"
#include "irr/debug_drawer.h"
#include "util/event_receiver_manager.h"
#include "finger_direction_event.h"
#include "ai_player.h"
#include "game/game_result.h"

using namespace std;
using namespace irr;

class AttackPhase : public BaseFingerDirectionPhase {
public:
	AttackPhase(irr::scene::ICameraSceneNode *cam);
	virtual ~AttackPhase();
	virtual std::unique_ptr<SubSequence> update(int ms);
	virtual std::unique_ptr<FingerResult> createResult(irr::scene::ICameraSceneNode *cam);
};

class DefensePhase : public BaseFingerDirectionPhase {
public:
	DefensePhase(irr::scene::ICameraSceneNode *cam);
	virtual ~DefensePhase();
	virtual std::unique_ptr<SubSequence> update(int ms);
	virtual std::unique_ptr<FingerResult> createResult(irr::scene::ICameraSceneNode *cam);
};


BaseFingerDirectionPhase::BaseFingerDirectionPhase(irr::scene::ICameraSceneNode *cam)
	: SubSequence(cam),
	end_(false),
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

BaseFingerDirectionPhase::~BaseFingerDirectionPhase()
{
	if(evtReceiver_ != nullptr) {
		Lib::eventReceiver->detachReceiver(evtReceiver_);
		evtReceiver_ = nullptr;
	}

	root_->remove();
	root_->drop();
	root_ = nullptr;
}

const FingerDirectionEvent &BaseFingerDirectionPhase::getPlayerChoice() const
{
	return evtReceiver_->inputEvt;
}

std::unique_ptr<BaseFingerDirectionPhase> BaseFingerDirectionPhase::createAttack(irr::scene::ICameraSceneNode *cam)
{
	return std::unique_ptr<BaseFingerDirectionPhase>(new AttackPhase(cam));
}

std::unique_ptr<BaseFingerDirectionPhase> BaseFingerDirectionPhase::createDefense(irr::scene::ICameraSceneNode *cam)
{
	return std::unique_ptr<BaseFingerDirectionPhase>(new DefensePhase(cam));
}


BaseFingerDirectionPhase *BaseFingerDirectionPhase::createAttackRawPtr(irr::scene::ICameraSceneNode *cam)
{
	return new AttackPhase(cam);
}

BaseFingerDirectionPhase *BaseFingerDirectionPhase::createDefenseRawPtr(irr::scene::ICameraSceneNode *cam)
{
	return new DefensePhase(cam);
}

///////////////////////////////////////

AttackPhase::AttackPhase(irr::scene::ICameraSceneNode *cam)
	: BaseFingerDirectionPhase(cam)
{
	centerText_->setText(L"Attack");

	evtReceiver_ = new FingerDirectionEventReceiver(true, false, true);
	Lib::eventReceiver->attachReceiver(evtReceiver_);
}
AttackPhase::~AttackPhase()
{
	
}

std::unique_ptr<SubSequence> AttackPhase::update(int ms)
{
	std::unique_ptr<SubSequence> empty;

	if(isEnd() == true) {
		return empty;
	}
	if(evtReceiver_->inputEvt.isValid() == false) {
		return empty;
	}
	FingerDirectionEvent aiEvt = AiPlayer().think(evtReceiver_->inputEvt);
	aiChoice_.reset(new FingerDirectionEvent(aiEvt));
	end_ = true;
	return empty;
}

std::unique_ptr<FingerResult> AttackPhase::createResult(irr::scene::ICameraSceneNode *cam)
{
	auto ai = this->getAiChoice();
	auto player = this->getPlayerChoice();
	return std::unique_ptr<FingerResult>(new AttackResult(cam, player, ai));
}

///////////////////////////////////////

DefensePhase::DefensePhase(irr::scene::ICameraSceneNode *cam)
	: BaseFingerDirectionPhase(cam)
{
	centerText_->setText(L"Defense");

	evtReceiver_ = new FingerDirectionEventReceiver(true, true, false);
	Lib::eventReceiver->attachReceiver(evtReceiver_);
}

DefensePhase::~DefensePhase()
{
}

std::unique_ptr<SubSequence> DefensePhase::update(int ms)
{
	std::unique_ptr<SubSequence> empty;
	if(isEnd() == true) {
		return empty;
	}
	if(evtReceiver_->inputEvt.isValid() == false) {
		return empty;
	}

	FingerDirectionEvent aiEvt = AiPlayer().think(evtReceiver_->inputEvt);
	aiChoice_.reset(new FingerDirectionEvent(aiEvt));
	end_ = true;
	return empty;
}

std::unique_ptr<FingerResult> DefensePhase::createResult(irr::scene::ICameraSceneNode *cam)
{
	auto ai = this->getAiChoice();
	auto player = this->getPlayerChoice();
	return std::unique_ptr<FingerResult>(new DefenseResult(cam, player, ai));
}