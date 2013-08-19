// Ŭnicode please 
#include "stdafx.h"
#include "ani_sequence.h"
#include "base/lib.h"
#include "irr/hmd_camera.h"

using namespace std;
using namespace irr;

namespace demo {;

AbstractHMDCameraEventReceiver *receiver = nullptr;

enum {
	kSimple=1000,
	kSpeed,
	kFrameLoop,
	kLoopMode,
};

AniSequence::AniSequence()
{
	auto smgr = Lib::smgr;
	
	auto cam = Lib::smgr->getActiveCamera();
	receiver = new HeadFreeCameraEventReceiver(cam, 0.1f, 0.1f);
	Lib::eventReceiver->attachReceiver(receiver);
	cam->setPosition(core::vector3df(0, 50, -200));

	irr::scene::IAnimatedMesh *dwarf = smgr->getMesh("ext/irrlicht/media/dwarf.x");

	{
		irr::scene::IAnimatedMeshSceneNode *base = smgr->addAnimatedMeshSceneNode(dwarf, nullptr, kSimple);
		base->setMaterialFlag(video::EMF_LIGHTING, false);
		base->setDebugDataVisible(true);
	}

	{
		irr::scene::IAnimatedMeshSceneNode *base = smgr->addAnimatedMeshSceneNode(dwarf, nullptr, kSpeed);
		base->setMaterialFlag(video::EMF_LIGHTING, false);
		base->setDebugDataVisible(true);
		base->setAnimationSpeed(10);
		base->setPosition(core::vector3df(100, 0, 0));
	}
	{
		irr::scene::IAnimatedMeshSceneNode *base = smgr->addAnimatedMeshSceneNode(dwarf, nullptr, kFrameLoop);
		base->setMaterialFlag(video::EMF_LIGHTING, false);
		base->setDebugDataVisible(true);
		base->setFrameLoop(10, 20);
		base->setPosition(core::vector3df(-100, 0, 0));
	}
	{
		irr::scene::IAnimatedMeshSceneNode *base = smgr->addAnimatedMeshSceneNode(dwarf, nullptr, kLoopMode);
		base->setMaterialFlag(video::EMF_LIGHTING, false);
		base->setDebugDataVisible(true);
		base->setFrameLoop(10, 20);
		base->setLoopMode(false);
		base->setPosition(core::vector3df(-200, 0, 0));
	}
}
AniSequence::~AniSequence()
{
	Lib::eventReceiver->detachReceiver(receiver);	
	receiver = nullptr;
}

void AniSequence::update(int ms)
{
	receiver->update(ms);

	static int elapsed = 0;
	elapsed += ms;

	if(elapsed > 1000) {
		elapsed = 0;
		irr::scene::IAnimatedMeshSceneNode *loopMode = static_cast<irr::scene::IAnimatedMeshSceneNode*>(Lib::smgr->getSceneNodeFromId(kLoopMode));
		loopMode->setLoopMode(!loopMode->getLoopMode());
	}
}

}	// namespace demo