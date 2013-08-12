// Ŭnicode please 
#include "stdafx.h"
#include "sphere_hmd_sequence.h"
#include "irr/sphere_attach_node.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"
#include "irr/debug_drawer.h"
#include "irr/text_3d_scene_node.h"
#include "util/event_receiver_manager.h"

using namespace std;
using namespace irr;

namespace demo {;

struct SphereSpriteInitData {
	float yaw;	//y축 기준. 좌우회전
	float pitch;	//x축 기준 회전. 위아래
	float radius;
};

irr::video::ITexture *rt = nullptr;

irr::f32 fRot = 0.0f;


class CameraEventReceiver : public ICustomEventReceiver {
public:
	CameraEventReceiver(irr::scene::ICameraSceneNode *cam)
		: cam_(cam),
		move_(0),
		strafe_(0)
	{
	}

	virtual bool OnEvent(const irr::SEvent &evt)
	{
		bool retval = false;

		if (evt.EventType == irr::EET_KEY_INPUT_EVENT) {
			if (evt.KeyInput.PressedDown) {
				switch (evt.KeyInput.Key) {
				case irr::KEY_KEY_W: 
					retval = true;
					move_ = 1; 
					break;
				case irr::KEY_KEY_S:
					retval = true;
					move_ = -1;
					break;
				case irr::KEY_KEY_A:
					retval = true;
					strafe_ = 1; 
					break;
				case irr::KEY_KEY_D:
					retval = true;
					strafe_ = -1; 
					break;
				default: 
					break;
				}
			} else {
				switch (evt.KeyInput.Key) {
				case irr::KEY_KEY_W: 
					if(move_ == 1) { 
						retval = true; 
						move_ = 0;
					} break;
				case irr::KEY_KEY_S: 
					if(move_ == -1) { 
						retval = true; 
						move_ = 0; 
					}
					break;
				case irr::KEY_KEY_A: 
					if(strafe_ == 1) { 
						retval = true; 
						strafe_ = 0; 
					}
					break;
				case irr::KEY_KEY_D: 
					if(strafe_ == -1) {
						retval = true; 
						strafe_ = 0; 
					}
					break;
				default: 
					break;
				}
			}
		}
		return retval;
	}

	virtual bool OnEvent(const SHeadTrackingEvent &evt)
	{
		return false;
	}

public:
	irr::s32 getMove() const { return move_; }
	irr::s32 getStrafe() const { return strafe_; }
	irr::scene::ICameraSceneNode *getCamera() { return cam_; }

private:
	//카메라이동관련. 일단 하드코딩
	irr::s32 move_;
	irr::s32 strafe_;
	irr::scene::ICameraSceneNode *cam_;
};

CameraEventReceiver *receiver = nullptr;

SphereHMDSequence::SphereHMDSequence()
{
	initScene();
	hideCursor();
	enableCollision();

	vector<SphereSpriteInitData> dataList;
	
	std::default_random_engine e1;
	std::uniform_real_distribution<float> yawRandGen(0, 360.0f);
	std::uniform_real_distribution<float> pitchRandGen(-80, 80);

	for(int i = 0 ; i < 25 ; i++) {
		SphereSpriteInitData data;
		data.yaw = yawRandGen(e1);
		data.pitch = pitchRandGen(e1);
		data.radius = 30 + i * 0.5f;
		dataList.push_back(data);
	}


	//billboard를 사용해서 sphere 효과를 낼수 있을거같은데...
	auto tex = Lib::driver->getTexture(res::texture::SORA2_PNG.c_str());

	for(auto &data : dataList) {
		auto node = Lib::smgr->addEmptySceneNode();
		node->setRotation(core::vector3df(data.pitch, data.yaw, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, tex);
		sprite->setSize(core::dimension2d<f32>(5, 5));
		sprite->setPosition(core::vector3df(0, 0, data.radius));
		node->addChild(sprite);
		sprite->drop();
		
		auto text = new irr::scene::Text3dSceneNode(node, Lib::smgr, 0, getNormalFont14(), L"sora!"); 
		text->setPosition(core::vector3df(0, 0, data.radius - 0.1f));
		auto size = core::dimension2d<f32>(5.0f, 5.0f);
		text->setSize(size);
		node->addChild(text);
		text->drop();
	}

	receiver = new CameraEventReceiver(Lib::smgr->getActiveCamera());
	Lib::eventReceiver->attachReceiver(receiver);
}
SphereHMDSequence::~SphereHMDSequence()
{
}

void SphereHMDSequence::update(int ms)
{
	auto cam = receiver->getCamera();
	irr::core::vector3df l_vPos  = cam->getPosition();
	irr::core::vector3df l_vTgt  = cam->getTarget() - l_vPos;
	irr::core::vector3df l_vUp   = cam->getUpVector();
	irr::core::vector3df l_vSide = l_vTgt.crossProduct(l_vUp);
	irr::core::vector3df l_vRot  = cam->getRotation();

	irr::f32 l_fMoveFactor = ((irr::f32)ms) / 10.0f;
	cam->setPosition(l_vPos + l_fMoveFactor * receiver->getMove() * l_vTgt + l_fMoveFactor * receiver->getStrafe() * l_vSide);
	//l_pCam->setRotation(irr::core::vector3df(l_vRot.X, l_vRot.Y - ((irr::f32)l_iMouseX) / 15.0f, l_vRot.Z + fRot));
}

}	// namespace demo