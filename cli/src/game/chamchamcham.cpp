// Ŭnicode please 
#include "stdafx.h"
#include "res.h"
#include "chamchamcham.h"
#include "irr/text_3d_scene_node.h"
#include "irr/sprite_scene_node.h"
#include "base/lib.h"
#include "irr/debug_drawer.h"
#include "util/event_receiver_manager.h"
#include "irr/head_tracker.h"
#include "irr/leapmotion.h"

using namespace std;
using namespace irr;

int ChamChamChamEvent::getDirectionCount() 
{
	//좌우/좌우상하 중에서 선택
	//return 2; 
	return 4;
}

bool ChamChamChamEvent::operator==(const ChamChamChamEvent &o) const
{
	return (this->value == o.value);
}
bool ChamChamChamEvent::operator!=(const ChamChamChamEvent &o) const
{
	return !(*this == o);
}

class LRUDEventReceiver : public ICustomEventReceiver {
public:
	virtual bool OnEvent(const irr::SEvent &evt) {
		if(evt.EventType == irr::EET_KEY_INPUT_EVENT) {
			if(evt.KeyInput.PressedDown) {
				switch(evt.KeyInput.Key) {
				case KEY_LEFT:
					inputEvt = ChamChamChamEvent::left();
					return true;
					break;
				case KEY_RIGHT:
					inputEvt = ChamChamChamEvent::right();
					return true;
					break;
				case KEY_UP:
					if(ChamChamChamEvent::getDirectionCount() > 2) {
						inputEvt = ChamChamChamEvent::up();
						return true;
					} else {
						inputEvt.value = ChamChamChamEvent::kNone;
					}
					break;
				case KEY_DOWN:
					if(ChamChamChamEvent::getDirectionCount() > 2) {
						inputEvt = ChamChamChamEvent::down();
						return true;
					} else {
						inputEvt.value = ChamChamChamEvent::kNone;
					}
					break;
				default:
					return false;
				}
			} else {
				inputEvt.value = ChamChamChamEvent::kNone;
			}
		}
		return false;
	}

	virtual bool OnEvent(const SHeadTrackingEvent &evt) {
		const float yawLimit = core::PI * 0.25f * 0.4f;
		const float pitchLimit = core::PI * 0.25f * 0.3f;
		if(evt.yaw > yawLimit) {
			//left
			inputEvt = ChamChamChamEvent::left();
			return true;
		} else if(evt.yaw < -yawLimit) {
			inputEvt = ChamChamChamEvent::right();
			//right
			return true;
		}
		if(ChamChamChamEvent::getDirectionCount() > 2) {
			if(evt.pitch > pitchLimit) {
				//up
				inputEvt = ChamChamChamEvent::up();
				return true;
			} else if(evt.pitch < -pitchLimit) {
				//down
				inputEvt = ChamChamChamEvent::down();
				return true;
			}
		}
		return false;
	}
#ifdef USE_LEAP_MOTION
	virtual bool OnEvent(const SLeapMotionEvent &evt) {
		auto gestures = evt.gestures;

		for(auto gesture : gestures)
		{
			if(gesture.type() == Leap::Gesture::TYPE_SWIPE)
			{
				const auto& swipe = static_cast<Leap::SwipeGesture>(gesture);
				auto vector = swipe.direction();

				// Float 비교 부정확성, 그리고 소수점 이하 비교는 필요하지 않기 떄문에
				// Int로 형변환해서 비교한다
				int x = 100 * vector.x;
				int y = 100 * vector.y;
				int absX = abs(x);
				int absY = abs(y);

				int absLargest = max(absX, absY);

				if(absLargest == x)
				{
					inputEvt = ChamChamChamEvent::right();
				}
				else if(absLargest == absX)
				{
					inputEvt = ChamChamChamEvent::left();
				}
				else if(absLargest == y)
				{
					inputEvt = ChamChamChamEvent::up();
				}
				else if(absLargest == absY)
				{
					inputEvt = ChamChamChamEvent::down();
				}

				return true;
			}
		}

		return false;
	}
#endif
	ChamChamChamEvent inputEvt;
};

///////////////////////////////////////

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
	if(ChamChamChamEvent::getDirectionCount() > 2) {
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
	if(ChamChamChamEvent::getDirectionCount() > 2) {
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

	evtReceiver_ = new LRUDEventReceiver();
	Lib::eventReceiver->attachReceiver(evtReceiver_);
}

BaseChamChamCham::~BaseChamChamCham()
{
	Lib::eventReceiver->detachReceiver(evtReceiver_);
	evtReceiver_ = nullptr;

	root_->remove();
	root_->drop();
	root_ = nullptr;
}

ChamChamChamEvent BaseChamChamCham::choiceAIEvent() const
{
	static std::default_random_engine e1(time(nullptr));
	std::uniform_int_distribution<int> randGen(1, ChamChamChamEvent::getDirectionCount());
	auto value = randGen(e1);
	return ChamChamChamEvent(value);
}

const ChamChamChamEvent &BaseChamChamCham::getPlayerChoice() const
{
	return evtReceiver_->inputEvt;
}


///////////////////////////////////////

ChamChamChamAttack::ChamChamChamAttack(irr::scene::ICameraSceneNode *cam)
	: BaseChamChamCham(cam)
{
	centerText_->setText(L"Attack");
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

	aiChoice_ = this->choiceAIEvent();
	end_ = true;
}

///////////////////////////////////////

ChamChamChamDefense::ChamChamChamDefense(irr::scene::ICameraSceneNode *cam)
	: BaseChamChamCham(cam)
{
	centerText_->setText(L"Defense");
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

	aiChoice_ = this->choiceAIEvent();
	end_ = true;
}
