// Ŭnicode please 
#pragma once

#include "demo_sequence.h"

namespace demo {;

class CylinderHMDSequence : public AbstractHMDSequence {
public:
	CylinderHMDSequence();
	virtual ~CylinderHMDSequence();

	virtual void update(int ms);
};

}	// namespace demo