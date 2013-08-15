// Ŭnicode please 
#pragma once

#include "game/sequence.h"

class UiTestSequence : public Sequence {
public:
	UiTestSequence();
	virtual ~UiTestSequence();

	virtual void update(int ms);
	virtual void preDraw();
	virtual void postDraw() {}
private:
};
