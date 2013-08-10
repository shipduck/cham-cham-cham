// Ŭnicode please 
#pragma once

#include "demo_sequence.h"

namespace demo {;
class SphereHMDSequence : public AbstractHMDSequence {
public:
	SphereHMDSequence();
	virtual ~SphereHMDSequence();

	virtual void update(int ms);
};
}	// namespace demo