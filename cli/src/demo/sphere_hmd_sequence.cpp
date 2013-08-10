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

}
SphereHMDSequence::~SphereHMDSequence()
{
}

void SphereHMDSequence::update(int ms)
{
}