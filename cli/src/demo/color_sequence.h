// Ŭnicode please 
#pragma once

#include "game/sequence.h"

namespace demo {;

class ColorSequence : public Sequence {
public:
	ColorSequence();
	virtual ~ColorSequence();

public:
	virtual void update(int ms);
	virtual void preDraw() {}
	virtual void postDraw() {}
};

}	// namespace demo