// Ŭnicode please 
#include "stdafx.h"
#include "demo_sequence_helper.h"
#include "base/lib.h"

using namespace std;
using namespace irr;

const std::vector<irr::SKeyMap> DemoSequenceHelper::getKeyMapData() const
{
	static std::vector<SKeyMap> keyMapArray;
	if(keyMapArray.size() == 0) {
		keyMapArray.push_back(SKeyMap(EKA_MOVE_FORWARD, irr::KEY_KEY_W));
		keyMapArray.push_back(SKeyMap(EKA_MOVE_BACKWARD, irr::KEY_KEY_S));
		keyMapArray.push_back(SKeyMap(EKA_STRAFE_LEFT, irr::KEY_KEY_A));
		keyMapArray.push_back(SKeyMap(EKA_STRAFE_RIGHT, irr::KEY_KEY_D));
		keyMapArray.push_back(SKeyMap(EKA_JUMP_UP, irr::KEY_SPACE));
		keyMapArray.push_back(SKeyMap(EKA_MOVE_FORWARD, irr::KEY_UP));
		keyMapArray.push_back(SKeyMap(EKA_MOVE_BACKWARD, irr::KEY_DOWN));
		keyMapArray.push_back(SKeyMap(EKA_STRAFE_LEFT, irr::KEY_LEFT));
		keyMapArray.push_back(SKeyMap(EKA_STRAFE_RIGHT, irr::KEY_RIGHT));
	}
	return keyMapArray;
}

DemoSequenceHelper::DemoSequenceHelper(Sequence *seq)
	: sequence_(seq),
	q3node_(nullptr)
{
}

void DemoSequenceHelper::loadSampleMap()
{
	bool retval = Lib::device->getFileSystem()->addFileArchive("ext/irrlicht/media/map-20kdm2.pk3");
	SR_ASSERT(retval == true);

	scene::IAnimatedMesh *q3levelmesh = Lib::smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode *q3node = nullptr;
	if(q3levelmesh) {
		q3node = Lib::smgr->addOctreeSceneNode(q3levelmesh->getMesh(0), 0, -1, 1024);
	}
	if(q3node) {
		q3node->setPosition(core::vector3df(-1350, -130, -1400));
	}
}
/*
void DemoSequenceHelper::loadSampleCollisionMap()
{
	loadSampleMap();
	scene::IMeshSceneNode *q3node = q3node_;

	scene::ITriangleSelector *selector = nullptr;
	if(q3node) {
		selector = Lib::smgr->createOctreeTriangleSelector(q3node->getMesh(), q3node, 128);
		q3node->setTriangleSelector(selector);
	}

	if(selector) {
		scene::ISceneNodeAnimator *anim = Lib::smgr->createCollisionResponseAnimator(selector, camera, core::vector3df(30, 50, 30), core::vector3df(0, -10, 0), core::vector3df(0, 30, 0));
		selector->drop();
		camera->addAnimator(anim);
		anim->drop();
	}	
}
*/

irr::scene::ICameraSceneNode *DemoSequenceHelper::createFPSCamera()
{
	auto keyMapData = getKeyMapData();
	scene::ICameraSceneNode *camera = Lib::smgr->addCameraSceneNodeFPS(nullptr, 
		100.0f, 
		0.3f, 
		128, 
		keyMapData.data(), 
		keyMapData.size(), 
		false, 
		3.0f);
	camera->setPosition(getDefaultCamPos());
	camera->setTarget(getDefaultCamTarget());
	return camera;
}

irr::core::vector3df DemoSequenceHelper::getDefaultCamPos()
{
	return core::vector3df(50, 50, -60);
	
}
irr::core::vector3df DemoSequenceHelper::getDefaultCamTarget()
{
	return core::vector3df(-70, 30, -60);
}