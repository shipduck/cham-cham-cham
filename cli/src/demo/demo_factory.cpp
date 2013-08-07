// Ŭnicode please 
#include "stdafx.h"
#include "demo_factory.h"

// sequence 
#include "cylinder_hmd_sequence.h"

using namespace std;

std::unique_ptr<Sequence> DemoFactory::create(DemoSequenceType type) const
{
	Sequence *seq = nullptr;
	switch(type) {
	case kDemoCylinderHMD:
		seq = new CylinderHMDSequence();
		break;
	default:
		break;
	}
	SR_ASSERT(seq != nullptr);
	return unique_ptr<Sequence>(seq);
}