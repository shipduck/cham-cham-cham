// Ŭnicode please 
#pragma once

#include "sequence.h"

class TitleSequence : public Sequence {
public:
	TitleSequence();
	virtual ~TitleSequence();

	virtual void update(int ms);
	virtual void preDraw();
	virtual void postDraw() {}
private:
};
