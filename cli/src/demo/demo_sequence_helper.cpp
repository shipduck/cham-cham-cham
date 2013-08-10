// Ŭnicode please 
#include "stdafx.h"
#include "demo_sequence_helper.h"
#include "base/lib.h"

using namespace std;
using namespace irr;

DemoSequenceHelper::DemoSequenceHelper(Sequence *seq)
	: sequence_(seq)
{
}

void DemoSequenceHelper::loadSampleCollisionMap()
{
	bool retval = Lib::device->getFileSystem()->addFileArchive("ext/irrlicht/media/map-20kdm2.pk3");
	SR_ASSERT(retval == true);

	scene::IAnimatedMesh *q3levelmesh = Lib::smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode *q3node = nullptr;
	if(q3levelmesh) {
		q3node = Lib::smgr->addOctreeSceneNode(q3levelmesh->getMesh(0), 0, -1, 1024);
	}
	scene::ITriangleSelector *selector = nullptr;
	if(q3node) {
		q3node->setPosition(core::vector3df(-1350, -130, -1400));
		selector = Lib::smgr->createOctreeTriangleSelector(q3node->getMesh(), q3node, 128);
		q3node->setTriangleSelector(selector);
	}

	std::array<SKeyMap, 9> keyMapArray;
	int keyCount = 0;
	keyMapArray[keyCount++] = SKeyMap(EKA_MOVE_FORWARD, irr::KEY_KEY_W);
	keyMapArray[keyCount++] = SKeyMap(EKA_MOVE_BACKWARD, irr::KEY_KEY_S);
	keyMapArray[keyCount++] = SKeyMap(EKA_STRAFE_LEFT, irr::KEY_KEY_A);
	keyMapArray[keyCount++] = SKeyMap(EKA_STRAFE_RIGHT, irr::KEY_KEY_D);
	keyMapArray[keyCount++] = SKeyMap(EKA_JUMP_UP, irr::KEY_SPACE);
	keyMapArray[keyCount++] = SKeyMap(EKA_MOVE_FORWARD, irr::KEY_UP);
	keyMapArray[keyCount++] = SKeyMap(EKA_MOVE_BACKWARD, irr::KEY_DOWN);
	keyMapArray[keyCount++] = SKeyMap(EKA_STRAFE_LEFT, irr::KEY_LEFT);
	keyMapArray[keyCount++] = SKeyMap(EKA_STRAFE_RIGHT, irr::KEY_RIGHT);

	scene::ICameraSceneNode *camera = Lib::smgr->addCameraSceneNodeFPS(nullptr, 
		100.0f, 
		0.3f, 
		128, 
		keyMapArray.data(), 
		keyMapArray.size(), 
		true, 
		3.0f);
	camera->setPosition(core::vector3df(50, 50, -60));
	camera->setTarget(core::vector3df(-70, 30, -60));
	if(selector) {
		scene::ISceneNodeAnimator *anim = Lib::smgr->createCollisionResponseAnimator(selector, camera, core::vector3df(30, 50, 30), core::vector3df(0, -10, 0), core::vector3df(0, 30, 0));
		selector->drop();
		camera->addAnimator(anim);
		anim->drop();
	}
	
}