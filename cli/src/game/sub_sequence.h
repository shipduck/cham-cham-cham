// Ŭnicode please 
#pragma once

namespace irr {;
namespace scene {;
class SpriteSceneNode;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class ScoreBoard;

class SubSequence {
public:
	SubSequence(irr::scene::ICameraSceneNode *cam, ScoreBoard *board);
	virtual ~SubSequence();
	
	// update처리후 다음 프레임부터 사용할 sub sequence를 반환.
	virtual std::unique_ptr<SubSequence> update(int ms) = 0;

public:
	irr::scene::ICameraSceneNode *getCamera() { return cam_; }

	const ScoreBoard *getScoreBoard() const { return scoreBoard_; }
	ScoreBoard *getScoreBoard() { return scoreBoard_; }

private:
	irr::scene::ICameraSceneNode *cam_;
	ScoreBoard *scoreBoard_;
};
