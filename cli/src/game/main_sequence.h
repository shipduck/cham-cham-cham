// Ŭnicode please 
#pragma once

#include "sequence.h"

class MainSequence : public Sequence {
public:
	MainSequence();
	virtual ~MainSequence();

	virtual void update(int ms);
	virtual void preDraw() {}
	virtual void postDraw() {}
};