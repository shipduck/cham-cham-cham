// Ŭnicode please 
#include "stdafx.h"
#include "sub_sequence.h"

using namespace std;
using namespace irr;


SubSequence::SubSequence(irr::scene::ICameraSceneNode *cam)
	: cam_(cam)
{
}

SubSequence::~SubSequence()
{
}

SubSequenceTransitTimer::SubSequenceTransitTimer(SubSequence *seq, int ms)
	: SubSequence(nullptr), seq_(seq), remain_(ms)
{
}

SubSequenceTransitTimer::~SubSequenceTransitTimer()
{
}

std::unique_ptr<SubSequence> SubSequenceTransitTimer::update(int ms)
{
	remain_ -= ms;
	if(remain_ < 0) {
		return std::move(seq_);
	} else {
		std::unique_ptr<SubSequence> empty;
		return empty;
	}
}