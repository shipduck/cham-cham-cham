// Ŭnicode please 
#include "stdafx.h"
#include "sequence_factory.h"

#include "game_sequence.h"
#include "debug_draw_sequence.h"

using namespace std;

std::unique_ptr<Sequence> SequenceFactory::create(SequenceType type) const
{
	Sequence *seq = nullptr;
	switch(type) {
	case kSequenceTitle:
		break;
	case kSequenceDebugDraw:
		seq = new DebugDrawSequence();
		break;
	case kSequenceGame:
		seq = new GameSequence();
		break;
	case kSequenceGameLoading:
		break;
	default:
		break;
	}
	SR_ASSERT(seq != nullptr);
	return unique_ptr<Sequence>(seq);
}