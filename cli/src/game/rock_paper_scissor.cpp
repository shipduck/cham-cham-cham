// Ŭnicode please 
#include "stdafx.h"
#include "rock_paper_scissor.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"
#include "res.h"
#include "util/event_receiver_manager.h"

using namespace std;
using namespace irr;

bool RPSEvent::operator==(const RPSEvent &o) const
{
	SR_ASSERT(o.value != kNone);
	SR_ASSERT(this->value != kNone);
	return this->value == o.value;
}

bool RPSEvent::operator>(const RPSEvent &o) const
{
	SR_ASSERT(o.value != kNone);
	SR_ASSERT(this->value != kNone);
	if(this->value == kRock && o.value == kScissor) {
		return true;
	}
	if(this->value == kScissor && o.value == kPaper) {
		return true;
	}
	if(this->value == kPaper && o.value == kRock) {
		return true;
	}
	return false;
}

bool RPSEvent::operator>=(const RPSEvent &o) const
{
	if(*this == o) {
		return true;
	}
	return (*this > o);
}

bool RPSEvent::operator<(const RPSEvent &o) const
{
	SR_ASSERT(o.value != kNone);
	SR_ASSERT(this->value != kNone);
	if(this->value == kScissor && o.value == kRock) {
		return true;
	}
	if(this->value == kPaper && o.value == kScissor) {
		return true;
	}
	if(this->value == kRock && o.value == kPaper) {
		return true;
	}
	return false;
}
bool RPSEvent::operator<=(const RPSEvent &o) const
{
	if(*this == o) {
		return true;
	}
	return (*this < o);
}

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

	RPSEvent rpsEvent;
};

RockPaperScissor::RockPaperScissor(irr::scene::ICameraSceneNode *cam)
	: root_(nullptr),
	evtReceiver_(nullptr),
	end_(false)
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

	attachEventReceiver();
}

RockPaperScissor::~RockPaperScissor()
{
	if(evtReceiver_ != nullptr) {
		detachEventReceiver();
	}

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

	root_->setVisible(b);
	if(b == true) {
		attachEventReceiver();
	} else {
		detachEventReceiver();
	}
}

bool RockPaperScissor::isEnable() const
{
	return root_->isVisible();
}

void RockPaperScissor::update(int ms)
{
	if(end_ == true) {
		return;
	}

	if(evtReceiver_ == nullptr) {
		return;
	}

	auto playerEvt = evtReceiver_->rpsEvent;
	if(playerEvt.value == RPSEvent::kNone) {
		return;
	}
	auto aiEvt = selectAIEvent();

	end_ = true;
	aiChoice_ = aiEvt;
	playerChoice_ = playerEvt;
}

RPSEvent RockPaperScissor::selectAIEvent() const
{
	static std::default_random_engine e1;
	std::uniform_int_distribution<int> randGen(RPSEvent::kRock, RPSEvent::kScissor);
	auto value = randGen(e1);
	return RPSEvent(value);
}