// Ŭnicode please 
#pragma once

class ScoreBoard {
public:
	ScoreBoard(irr::scene::ICameraSceneNode *cam);
	~ScoreBoard();

public:
	int getPlayerScore() const { return playerScore_; }
	int getAiScore() const { return aiScore_; }

	//점수에 따라서 점수 스프라이트를 켜고끄기
	void update();
	bool isGameOver() const;

	void playerGetPoint() { playerScore_ += 1; }
	void aiGetPoint() { aiScore_ += 1; }

	void reset();

private:
	void updateScoreNode(int playerScore, int aiScore);

private:
	int playerScore_;
	int aiScore_;

	irr::scene::ISceneNode *hmdNode_;

	std::vector<irr::scene::ISceneNode*> playerScoreNodeList_;
	std::vector<irr::scene::ISceneNode*> aiScoreNodeList_;
};