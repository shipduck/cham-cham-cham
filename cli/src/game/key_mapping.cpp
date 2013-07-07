// Ŭnicode please 
#include "stdafx.h"
#include "key_mapping.h"

using namespace irr;

KeyMapping::KeyMapping()
{
	//forward
	KeyMapData forwardA;
	forwardA.keyCode = irr::KEY_UP;
	forwardKeyList_.push_back(forwardA);

	KeyMapData forwardB;
	forwardB.keyCode = irr::KEY_KEY_W;
	forwardKeyList_.push_back(forwardB);

	KeyMapData backwardA;
	backwardA.keyCode = irr::KEY_DOWN;
	backwardKeyList_.push_back(backwardA);

	KeyMapData backwardB;
	backwardB.keyCode = irr::KEY_KEY_S;
	backwardKeyList_.push_back(backwardB);

	KeyMapData leftA;
	leftA.keyCode = irr::KEY_LEFT;
	leftKeyList_.push_back(leftA);

	KeyMapData leftB;
	leftB.keyCode = irr::KEY_KEY_A;
	leftKeyList_.push_back(leftB);

	KeyMapData rightA;
	rightA.keyCode = irr::KEY_RIGHT;
	rightKeyList_.push_back(rightA);

	KeyMapData rightB;
	rightB.keyCode = irr::KEY_KEY_D;
	rightKeyList_.push_back(rightB);
}