// Ŭnicode please 
#pragma once

namespace irr {;
namespace scene {;
class SpriteSceneNode;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class SubSequence {
public:
	SubSequence(irr::scene::ICameraSceneNode *cam);
	virtual ~SubSequence();
	
	// update처리후 다음 프레임부터 사용할 sub sequence를 반환.
	virtual std::unique_ptr<SubSequence> update(int ms) = 0;

public:
	irr::scene::ICameraSceneNode *getCamera() { return cam_; }
	const irr::scene::ICameraSceneNode *getCamera() const { return cam_; }

private:
	irr::scene::ICameraSceneNode *cam_;

};

class SubSequenceTransitTimer : public SubSequence {
public:
	SubSequenceTransitTimer(SubSequence *seq, int ms);
	virtual ~SubSequenceTransitTimer();

	virtual std::unique_ptr<SubSequence> update(int ms);

private:
	std::unique_ptr<SubSequence> seq_;
	int remain_;
};