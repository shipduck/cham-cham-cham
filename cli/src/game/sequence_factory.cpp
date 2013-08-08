// Ŭnicode please 
#include "stdafx.h"
#include "sequence_factory.h"

#include "game_sequence.h"
#include "debug_draw_sequence.h"
#include "game_loading_sequence.h"
#include "title_sequence.h"

// demo
#include "demo/cylinder_hmd_sequence.h"

using namespace std;

std::unique_ptr<Sequence> SequenceFactory::create(SequenceType type) const
{
	Sequence *seq = nullptr;
	switch(type) {
	case kSequenceTitle:
		seq = new TitleSequence();
		break;
	case kSequenceDebugDraw:
		seq = new DebugDrawSequence();
		break;
	case kSequenceGame:
		seq = new GameSequence();
		break;
	case kSequenceGameLoading:
		seq = new GameLoadingSequence();
		break;
	default:
		break;
	}
	SR_ASSERT(seq != nullptr);
	return unique_ptr<Sequence>(seq);
}

std::unique_ptr<Sequence> SequenceFactory::create(DemoSequenceType type) const
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