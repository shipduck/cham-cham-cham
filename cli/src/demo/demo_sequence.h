// Ŭnicode please 
#pragma once

#include "game/sequence.h"

namespace demo {;
class AbstractHMDSequence : public Sequence {
public:
	AbstractHMDSequence();
	virtual ~AbstractHMDSequence();

public:
	enum {
		kIDLevel=1000,
		kIDFaerie,
		kIDDwarf,
	};

public:
	virtual void preDraw() {}
	virtual void postDraw() {}
	void initScene();
	void hideCursor();

	void enableCollision();
public:
	//hmd에서 사용하는 카메라 구현이 임시로 사용
	//실제로 게임에서 사용할 카멜 구현할때 참고용으로 두기
	static std::vector<irr::SKeyMap> getKeyMapData();
};

}	// namespace demo;
