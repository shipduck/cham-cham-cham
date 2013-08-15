// Ŭnicode please 
#pragma once

class ScoreBoard {
public:
	ScoreBoard(irr::scene::ICameraSceneNode *cam);
	~ScoreBoard();

public:
	int getPlayerScore() const { return playerScore_; }
	int getAiScore() const { return aiScore_; }

private:
	int playerScore_;
	int aiScore_;

	irr::scene::ISceneNode *hmdNode_;
};