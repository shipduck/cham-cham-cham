// Ŭnicode please 
#include "stdafx.h"
#include "demo_sequence.h"
#include "base/lib.h"

using namespace std;
using namespace irr;

namespace demo {;
AbstractHMDSequence::AbstractHMDSequence()
{
}

AbstractHMDSequence::~AbstractHMDSequence()
{
}

void AbstractHMDSequence::hideCursor()
{
	Lib::device->getCursorControl()->setVisible(false);
}

void AbstractHMDSequence::initScene()
{
	auto device = Lib::device;
	auto driver = device->getVideoDriver();
	auto smgr = device->getSceneManager();

	// Create world
	// fps camera node를 쓰면 StereoRenderer+HeadTracking 박살난다
	smgr->addCameraSceneNode();

	// load the quake map
	printf("%i\n", (int)device->getFileSystem()->addFileArchive("ext/irrlicht/media/map-20kdm2.pk3"));
	irr::scene::IAnimatedMesh *q3mesh = smgr->getMesh("20kdm2.bsp");
	irr::scene::ISceneNode *q3levelNode = smgr->addOctreeSceneNode(q3mesh->getMesh(0), 0, -1, 128);
	q3levelNode->setPosition(irr::core::vector3df(-1350,-90,-1400));

	// load a faerie
	irr::scene::IAnimatedMesh *faerie = smgr->getMesh("ext/irrlicht/media/faerie.md2");
	irr::scene::IAnimatedMeshSceneNode *faerieNode = smgr->addAnimatedMeshSceneNode(faerie);
	faerieNode->setMaterialTexture(0, driver->getTexture("ext/irrlicht/media/faerie2.bmp"));
	faerieNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	faerieNode->setPosition(irr::core::vector3df(40,190,-1030));
	faerieNode->setRotation(irr::core::vector3df(0,-90,0));
	faerieNode->setMD2Animation(irr::scene::EMAT_SALUTE);

	// load a dwarf
	irr::scene::IAnimatedMesh *dwarf = smgr->getMesh("ext/irrlicht/media/dwarf.x");
	irr::scene::IAnimatedMeshSceneNode *dwarfNode = smgr->addAnimatedMeshSceneNode(dwarf);
	dwarfNode->setPosition(irr::core::vector3df(40,-25,20));
}

std::vector<irr::SKeyMap> AbstractHMDSequence::getKeyMapData()
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

void AbstractHMDSequence::enableCollision()
{
	scene::ITriangleSelector *selector = nullptr;
	irr::scene::IMeshSceneNode *q3node = static_cast<irr::scene::IMeshSceneNode*>(Lib::smgr->getSceneNodeFromId(kIDLevel));
	if(q3node) {
		selector = Lib::smgr->createOctreeTriangleSelector(q3node->getMesh(), q3node, 128);
		q3node->setTriangleSelector(selector);
	}

	auto camera = Lib::smgr->getActiveCamera();
	if(selector) {
		scene::ISceneNodeAnimator *anim = Lib::smgr->createCollisionResponseAnimator(selector, camera, core::vector3df(30, 50, 30), core::vector3df(0, -10, 0), core::vector3df(0, 30, 0));
		selector->drop();
		camera->addAnimator(anim);
		anim->drop();
	}	
}

}	// namespace demo
