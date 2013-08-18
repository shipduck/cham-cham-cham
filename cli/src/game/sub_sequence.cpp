// Ŭnicode please 
#include "stdafx.h"
#include "sub_sequence.h"

using namespace std;
using namespace irr;


SubSequence::SubSequence(irr::scene::ICameraSceneNode *cam, ScoreBoard *board)
	: cam_(cam),
	scoreBoard_(board)
{
}

SubSequence::~SubSequence()
{
}
