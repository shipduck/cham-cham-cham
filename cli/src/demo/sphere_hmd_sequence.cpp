// Ŭnicode please 
#include "stdafx.h"
#include "sphere_hmd_sequence.h"
#include "irr/sphere_attach_node.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"
#include "irr/debug_drawer.h"
#include "irr/text_3d_scene_node.h"
#include "util/event_receiver_manager.h"
#include "irr/hmd_camera.h"

using namespace std;
using namespace irr;

namespace demo {;

struct SphereSpriteInitData {
	float yaw;	//y축 기준. 좌우회전
	float pitch;	//x축 기준 회전. 위아래
	float radius;
};

irr::video::ITexture *rt = nullptr;


CameraEventReceiver *receiver = nullptr;

SphereHMDSequence::SphereHMDSequence()
{
	initScene();
	hideCursor();
	enableCollision();

	auto cam = Lib::smgr->getActiveCamera();
	receiver = new CameraEventReceiver(cam);
	Lib::eventReceiver->attachReceiver(receiver);
	//cam->bindTargetAndRotation(true);
	//cam->setPosition(core::vector3df(0, 0, 0));
	//cam->setTarget(core::vector3df(0, 0, 1));

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

	auto hmdNode = Lib::smgr->addEmptySceneNode(cam);
	hmdNode->setRotation(core::vector3df(0, 180, 0));

	for(auto &data : dataList) {
		auto node = Lib::smgr->addEmptySceneNode(hmdNode);
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

	//처다보는 방향을 그리는데 사용할 간단한 billboard
	{
		auto tex = Lib::driver->getTexture("ext/irrlicht/media/particle.bmp");
		auto node = Lib::smgr->addEmptySceneNode(hmdNode);
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, tex);
		node->addChild(sprite);
		sprite->setSize(core::dimension2d<f32>(2, 2));
		sprite->setPosition(core::vector3df(0, 0, 50));
		sprite->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		sprite->drop();
	}

	//auto cube = Lib::smgr->addCubeSceneNode();
	//cube->setMaterialFlag(video::EMF_LIGHTING, false);
	//cube->setPosition(core::vector3df(+20, 0, 0));
}
SphereHMDSequence::~SphereHMDSequence()
{
}

void SphereHMDSequence::update(int ms)
{	
	receiver->update(ms);
}

}	// namespace demo