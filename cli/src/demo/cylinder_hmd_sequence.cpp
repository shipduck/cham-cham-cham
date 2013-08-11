// Ŭnicode please 
#include "stdafx.h"
#include "cylinder_hmd_sequence.h"
#include "irr/cylinder_mapping_node.h"
#include "base/lib.h"
#include "irr/head_tracker.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

namespace demo {;

irr::scene::ISceneNode *node = nullptr;

struct CylinderMappingNodeData {
	float radius;

	//cylinder texture node가 붙을 위치를 지정
	float posY;
	float rotate;

	//cylinder texture node가 붙은 다음에 어떤 행동을 할지 지정
	core::vector3df rotationAnim;
};

CylinderHMDSequence::CylinderHMDSequence()
{
	initScene();
	hideCursor();
	enableCollision();

	auto camNode = Lib::smgr->getActiveCamera();	
	node = Lib::smgr->addEmptySceneNode();
	node->setPosition(camNode->getPosition());

	//랜덤생성은 C++을 좀 써보자
	std::default_random_engine e1;
	std::uniform_real_distribution<float> posYRandGen(-30.f, 30.0f);
	std::uniform_real_distribution<float> rotateRandGen(0, 360.0f);
	std::uniform_real_distribution<float> basicRandGen(0, 1.0f);

	vector<CylinderMappingNodeData> dataList;
	for(int i = 0 ; i < 30 ; i++) {
		CylinderMappingNodeData data;
		data.radius = i*0.5f + 40.0f;
		data.posY = posYRandGen(e1);
		data.rotate = rotateRandGen(e1);

		float rotateRatio = 0.3f;
		float randRotate = basicRandGen(e1);
		if(randRotate < rotateRatio) {
			if(rotateRatio / 2.0f < randRotate) {
				data.rotationAnim = core::vector3df(0, 0.3f,0);
			} else {
				data.rotationAnim = core::vector3df(0, -0.3f,0);
			}
		}
		dataList.push_back(data);
	}

	auto tex = Lib::driver->getTexture(res::texture::SORA2_PNG.c_str());
	for(auto &data : dataList) {
		auto cylinderNode = new CylinderMappingNode(node, Lib::smgr, 0, tex);
		cylinderNode->setPosition(core::vector3df(0, data.posY, 0));
		cylinderNode->setRotation(core::vector3df(0, data.rotate, 0));
		if(data.rotationAnim != core::vector3df(0, 0, 0)) {
			auto anim = Lib::smgr->createRotationAnimator(data.rotationAnim);
			cylinderNode->addAnimator(anim);
			anim->drop();
		}
		cylinderNode->radius = data.radius;
		cylinderNode->drop();
	}
}
CylinderHMDSequence::~CylinderHMDSequence()
{
}

void CylinderHMDSequence::update(int ms)
{
	//HMD 테스트 목적이기 일단은 카메라와 HMD는 좌표만 동기화 시키면된다
	//회전까지는 동기화 하지 않아도 될듯. 근데 카메라 좌표계 변동 로직을 확실히 하지 않아서 밀리는 느낌
	auto camNode = Lib::smgr->getActiveCamera();
	SR_ASSERT(camNode != nullptr);
	node->setPosition(camNode->getPosition());
}
}	// namespace demo