// Ŭnicode please 
#include "stdafx.h"
#include "sphere_hmd_sequence.h"
#include "irr/sphere_attach_node.h"
#include "base/lib.h"
#include "demo_sequence_helper.h"

using namespace std;
using namespace irr;

SphereHMDSequence::SphereHMDSequence()
{
	DemoSequenceHelper helper(this);
	helper.loadSampleCollisionMap();
	Lib::device->getCursorControl()->setVisible(false);

	auto cam = Lib::smgr->getActiveCamera();
	SR_ASSERT(cam != nullptr);
}
SphereHMDSequence::~SphereHMDSequence()
{
}

void SphereHMDSequence::update(int ms)
{
}