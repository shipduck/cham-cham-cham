// Ŭnicode please 
#pragma once

#include "game/sequence.h"

class CylinderHMDSequence : public Sequence {
public:
	CylinderHMDSequence();
	virtual ~CylinderHMDSequence();

	virtual void update(int ms);
};