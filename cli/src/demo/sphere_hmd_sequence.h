// Ŭnicode please 
#pragma once

#include "game/sequence.h"

class SphereHMDSequence : public Sequence {
public:
	SphereHMDSequence();
	virtual ~SphereHMDSequence();

	virtual void update(int ms);
	virtual void preDraw() {}
	virtual void postDraw() {}
};