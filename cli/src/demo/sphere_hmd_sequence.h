// Ŭnicode please 
#pragma once

#include "demo_sequence.h"

class AbstractHMDCameraEventReceiver;
namespace demo {;
class SphereHMDSequence : public AbstractHMDSequence {
public:
	SphereHMDSequence();
	virtual ~SphereHMDSequence();

	virtual void update(int ms);
private:
	AbstractHMDCameraEventReceiver *receiver;
};
}	// namespace demo