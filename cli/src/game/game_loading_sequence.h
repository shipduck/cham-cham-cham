// Ŭnicode please 
#pragma once

#include "sequence.h"

class GameLoadingSequence : public Sequence {
public:
	GameLoadingSequence();
	virtual ~GameLoadingSequence();
	
	virtual void update(int ms);
	virtual void preDraw() {}
	virtual void postDraw() {}
};