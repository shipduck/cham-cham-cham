// Ŭnicode please 
#include "stdafx.h"
#include "sphere_hmd_sequence.h"
#include "irr/sphere_attach_node.h"
#include "base/lib.h"

using namespace std;
using namespace irr;

SphereHMDSequence::SphereHMDSequence()
{
	// add fps camera
	auto camNode = Lib::smgr->addCameraSceneNodeFPS();
	Lib::device->getCursorControl()->setVisible(false);

	bool retval = Lib::device->getFileSystem()->addFileArchive("ext/irrlicht/media/map-20kdm2.pk3");
	SR_ASSERT(retval == true);

	scene::IAnimatedMesh *mesh = Lib::smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode *node = nullptr;
	if(mesh) {
		node = Lib::smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
	}
	if(node) {
		node->setPosition(core::vector3df(-1300, -144, -1249));
	}	
}
SphereHMDSequence::~SphereHMDSequence()
{
}

void SphereHMDSequence::update(int ms)
{
}