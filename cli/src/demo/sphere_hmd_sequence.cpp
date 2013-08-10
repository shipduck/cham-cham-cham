// Ŭnicode please 
#include "stdafx.h"
#include "sphere_hmd_sequence.h"
#include "irr/sphere_attach_node.h"
#include "base/lib.h"
#include "util/event_receiver_manager.h"

using namespace std;
using namespace irr;

namespace demo {;

SphereHMDSequence::SphereHMDSequence()
{
	initScene();
	hideCursor();
	enableCollision();
}
SphereHMDSequence::~SphereHMDSequence()
{
}

void SphereHMDSequence::update(int ms)
{
}

}	// namespace demo