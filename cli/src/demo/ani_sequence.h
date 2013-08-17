// Ŭnicode please 
#pragma once

#include "game/sequence.h"

namespace demo {;

class AniSequence : public Sequence {
public:
	AniSequence();
	virtual ~AniSequence();

public:
	virtual void update(int ms);
	virtual void preDraw() {}
	virtual void postDraw() {}
};

}	// namespace demo