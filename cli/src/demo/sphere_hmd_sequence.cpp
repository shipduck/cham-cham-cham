// Ŭnicode please 
#include "stdafx.h"
#include "sphere_hmd_sequence.h"
#include "irr/sphere_attach_node.h"
#include "base/lib.h"
#include "demo_sequence_helper.h"
#include "util/event_receiver_manager.h"

using namespace std;
using namespace irr;

SphereHMDSequence::SphereHMDSequence()
{
	auto device = Lib::device;
	auto driver = device->getVideoDriver();
	auto smgr = device->getSceneManager();

	// Create world
	// fps camera node를 쓰면 StereoRenderer+HeadTracking 박살난다
	smgr->addCameraSceneNode();

	// load the quake map
	printf("%i\n", (int)device->getFileSystem()->addFileArchive("ext/irrlicht/media/map-20kdm2.pk3"));
	irr::scene::IAnimatedMesh *l_pMesh      = smgr->getMesh("20kdm2.bsp");
	irr::scene::ISceneNode    *l_pLevelNode = smgr->addOctreeSceneNode(l_pMesh->getMesh(0), 0, -1, 128);
	l_pLevelNode->setPosition(irr::core::vector3df(-1350,-90,-1400));

	// load a faerie
	irr::scene::IAnimatedMesh          *l_pFaerie     = smgr->getMesh("ext/irrlicht/media/faerie.md2");
	irr::scene::IAnimatedMeshSceneNode *l_pFaerieNode = smgr->addAnimatedMeshSceneNode(l_pFaerie);
	l_pFaerieNode->setMaterialTexture(0, driver->getTexture("ext/irrlicht/media/faerie2.bmp"));
	l_pFaerieNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	l_pFaerieNode->setPosition(irr::core::vector3df(40,190,-1030));
	l_pFaerieNode->setRotation(irr::core::vector3df(0,-90,0));
	l_pFaerieNode->setMD2Animation(irr::scene::EMAT_SALUTE);

	// load a dwarf
	irr::scene::IAnimatedMesh          *l_pDwarf     = smgr->getMesh("ext/irrlicht/media/dwarf.x");
	irr::scene::IAnimatedMeshSceneNode *l_pDwarfNode = smgr->addAnimatedMeshSceneNode(l_pDwarf);
	l_pDwarfNode->setPosition(irr::core::vector3df(40,-25,20));

	device->getCursorControl()->setVisible(false);
}
SphereHMDSequence::~SphereHMDSequence()
{
}

void SphereHMDSequence::update(int ms)
{
}