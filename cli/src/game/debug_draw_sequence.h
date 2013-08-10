// Ŭnicode please 
#pragma once

#include "sequence.h"

class DebugDrawManager2D;
class DebugDrawManager3D;

class DebugDrawSequence : public Sequence {
public:
	DebugDrawSequence();
	virtual ~DebugDrawSequence();

	virtual void update(int ms);
	virtual void preDraw() {}
	virtual void postDraw() {}
};

