// Ŭnicode please 
#include "stdafx.h"
#include "sphere_hmd_sequence.h"
#include "irr/sphere_attach_node.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"

using namespace std;
using namespace irr;

namespace demo {;

struct SphereSpriteInitData {
	float yaw;	//y축 기준. 좌우회전
	float pitch;	//x축 기준 회전. 위아래
	float radius;
};

SphereHMDSequence::SphereHMDSequence()
{
	initScene();
	hideCursor();
	enableCollision();

	vector<SphereSpriteInitData> dataList;
	for(int i = 0 ; i < 25 ; i++) {
		SphereSpriteInitData data;
		data.yaw = ((float)rand() / RAND_MAX) * 360;
		data.pitch = (((float)rand() / RAND_MAX) * 160) - 80;
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
	}
}
SphereHMDSequence::~SphereHMDSequence()
{
}

void SphereHMDSequence::update(int ms)
{
}

}	// namespace demo