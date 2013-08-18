// Ŭnicode please 
#include "stdafx.h"
#include "rock_paper_scissor.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"
#include "res.h"
#include "util/event_receiver_manager.h"
#include "irr/leapmotion.h"
#include "rps_event.h"
#include "ai_player.h"
#include "game/game_result.h"

using namespace std;
using namespace irr;


class RPSEventReceiver : public ICustomEventReceiver {
public:
	RPSEventReceiver() {
	}
	virtual ~RPSEventReceiver() {
	}

	virtual bool OnEvent(const irr::SEvent &evt) {
		if(evt.EventType == irr::EET_KEY_INPUT_EVENT) {
			if(evt.KeyInput.PressedDown) {
				switch(evt.KeyInput.Key) {
				case KEY_LEFT:
					rpsEvent.value = RPSEvent::kRock;
					return true;
					break;
				case KEY_UP:
					rpsEvent.value = RPSEvent::kPaper;
					return true;
					break;
				case KEY_RIGHT:
					rpsEvent.value = RPSEvent::kScissor;
					return true;
					break;
				default:
					return false;
				}
			} else {
				rpsEvent.value = RPSEvent::kNone;
			}
		}
		return false;
	}
	virtual bool OnEvent(const SHeadTrackingEvent &evt) {
		return false;
	}

#ifdef USE_LEAP_MOTION
	virtual bool OnEvent(const SLeapMotionEvent &evt) {
		int noFingers = evt.fingers.count();
		if(noFingers >= 3) {
			rpsEvent.value = RPSEvent::kPaper;
		}
		else if(noFingers == 1 || noFingers == 2) {
			rpsEvent.value = RPSEvent::kScissor;
		}
		else {
			rpsEvent.value = RPSEvent::kRock;
		}
		
		return true;
	}
#endif

	RPSEvent rpsEvent;
};

RockPaperScissor::RockPaperScissor(irr::scene::ICameraSceneNode *cam)
	: SubSequence(cam),
	root_(nullptr),
	evtReceiver_(nullptr),
	end_(false),
	enable_(true)
{
	root_ = Lib::smgr->addEmptySceneNode(cam);
	root_->grab();
	root_->setRotation(core::vector3df(0, 180, 0));

	std::vector<irr::scene::SpriteSceneNode*> rpsSpriteList;
	std::vector<irr::scene::SpriteSceneNode*> padIconList;

	const bool usePadIcon = false;

	//생성한 sprite를 drop하는것은 list 루프에서 처리함
	{
		auto rockTex = Lib::driver->getTexture(res::texture::ICON_ROCK_PNG);
		rock_ = Lib::smgr->addEmptySceneNode(root_);
		rock_->setRotation(core::vector3df(17, -13, 0));
		auto sprite = new irr::scene::SpriteSceneNode(rock_, Lib::smgr, 0, rockTex);
		rpsSpriteList.push_back(sprite);
		
		if(usePadIcon) {
			auto xTex = Lib::driver->getTexture(res::texture::XBOX_PAD_X_PNG);
			auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, xTex);
			padIconList.push_back(icon);
		}
	}
	{
		auto paperTex = Lib::driver->getTexture(res::texture::ICON_PAPER_PNG);
		scissor_ = Lib::smgr->addEmptySceneNode(root_);
		scissor_->setRotation(core::vector3df(17, 0, 0));
		auto sprite = new irr::scene::SpriteSceneNode(scissor_, Lib::smgr, 0, paperTex);
		rpsSpriteList.push_back(sprite);

		if(usePadIcon) {
			auto iconTex = Lib::driver->getTexture(res::texture::XBOX_PAD_Y_PNG);
			auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, iconTex);
			padIconList.push_back(icon);
		}
	}
	{
		auto scissorTex = Lib::driver->getTexture(res::texture::ICON_SCISSOR_PNG);
		paper_ = Lib::smgr->addEmptySceneNode(root_);
		paper_->setRotation(core::vector3df(17, 13, 0));
		auto sprite = new irr::scene::SpriteSceneNode(paper_, Lib::smgr, 0, scissorTex);
		rpsSpriteList.push_back(sprite);

		if(usePadIcon) {
			auto iconTex = Lib::driver->getTexture(res::texture::XBOX_PAD_B_PNG);
			auto icon = new irr::scene::SpriteSceneNode(sprite, Lib::smgr, 0, iconTex);
			padIconList.push_back(icon);
		}
	}

	for(auto sprite : rpsSpriteList) {
		float radius = 50;
		core::vector3df spritePos(0, 0, radius);
		core::dimension2d<float> rpsSize(4, 4);
		sprite->setSize(rpsSize);
		sprite->setPosition(spritePos);
		sprite->drop();
	}

	for(auto icon : padIconList) {
		icon->setPosition(core::vector3df(0, -3, -0.5));
		icon->setSize(core::dimension2d<float>(2, 2));
		icon->drop();
	}

	attachEventReceiver();
}

RockPaperScissor::~RockPaperScissor()
{
	if(evtReceiver_ != nullptr) {
		detachEventReceiver();
	}

	root_->remove();
	root_->drop();
	root_ = nullptr;
}

void RockPaperScissor::attachEventReceiver()
{
	SR_ASSERT(evtReceiver_ == nullptr);
	evtReceiver_ = new RPSEventReceiver();
	Lib::eventReceiver->attachReceiver(evtReceiver_);
}
void RockPaperScissor::detachEventReceiver()
{
	SR_ASSERT(evtReceiver_ != nullptr);
	Lib::eventReceiver->detachReceiver(evtReceiver_);
	evtReceiver_ = nullptr;
}

	
void RockPaperScissor::setEnable(bool b)
{
	if(isEnable() == b) {
		return;
	}

	enable_ = b;
	if(b == true) {
		attachEventReceiver();
	} else {
		detachEventReceiver();
	}
}

std::unique_ptr<SubSequence> RockPaperScissor::update(int ms)
{
	std::unique_ptr<SubSequence> empty;

	if(end_ == true) {
		return empty;
	}

	if(evtReceiver_ == nullptr) {
		return empty;
	}

	auto playerEvt = evtReceiver_->rpsEvent;
	if(playerEvt.value == RPSEvent::kNone) {
		return empty;
	}
	auto aiEvt = AiPlayer().think(playerEvt);

	end_ = true;
	aiChoice_.reset(new RPSEvent(aiEvt));
	playerChoice_.reset(new RPSEvent(playerEvt));

	//return std::unique_ptr<SubSequence>(new SubSequenceTransitTimer(new RockPaperScissorResult(getCamera(), playerEvt, aiEvt)));
	return std::unique_ptr<SubSequence>(new RockPaperScissorResult(getCamera(), playerEvt, aiEvt));
}
