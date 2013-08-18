// Ŭnicode please 
#include "stdafx.h"
#include "score_board.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"
#include "res.h"

using namespace std;
using namespace irr;

//const int maxScore = 2;
const int maxScore = 3;
static_assert(maxScore > 0, "min score==1");

struct ScoreBoardSpriteData {
	ScoreBoardSpriteData(float yaw=0.0f, 
		float pitch=0.0f, 
		float sizeX=1.0f, 
		float sizeY=1.0f, 
		float radius=1.0f, 
		irr::video::ITexture *tex=nullptr)
		: yaw(yaw),
		pitch(pitch),
		sizeX(sizeX),
		sizeY(sizeY),
		radius(radius),
		tex(tex)
	{
	}

	float yaw;	//y축 기준. 좌우회전
	float pitch;	//x축 기준 회전. 위아래
	float sizeX;
	float sizeY;
	float radius;
	irr::video::ITexture *tex;
};

ScoreBoard::ScoreBoard(irr::scene::ICameraSceneNode *cam)
	: playerScore_(0),
	aiScore_(0),
	hmdNode_(nullptr)
{
	auto reinaTex = Lib::driver->getTexture(res::texture::FACE_REINA_PNG);
	auto playerTex = Lib::driver->getTexture(res::texture::FACE_PLAYER_PNG);
	auto circleGreen = Lib::driver->getTexture(res::texture::CIRCLE_GREEN_PNG);
	auto circleRed = Lib::driver->getTexture(res::texture::CIRCLE_RED_PNG);
	auto circleGray = Lib::driver->getTexture(res::texture::CIRCLE_GRAY_PNG);

	const float yawSeperation = 10.0f;
	const float pitchSeperation = 10.0f;
	const float scoreboardYaw = 20.0f;
	const float scoreboardPitch = -25.0f;

	vector<ScoreBoardSpriteData> dataList;
	const ScoreBoardSpriteData reina(scoreboardYaw, scoreboardPitch, 2, 2, 30, reinaTex);
	dataList.push_back(reina);
	
	const ScoreBoardSpriteData player(scoreboardYaw, scoreboardPitch + pitchSeperation, 2, 2, 30, playerTex);
	dataList.push_back(player);
	
	const ScoreBoardSpriteData bgCircle(0, 0, 2.0f, 2.0f, 30.0f, circleGray);
	const ScoreBoardSpriteData reinaWinCircle(0, 0, 1.5f, 1.5f, 25.0f, circleRed);
	const ScoreBoardSpriteData playerWinCircle(0, 0, 1.5f, 1.5f, 25.0f, circleGreen);

	for(int i = 0 ; i < maxScore ; i++) {
		for(int j = 0 ; j < 2 ; j++) {
			float yaw = scoreboardYaw + yawSeperation * (i + 1);
			float pitch = scoreboardPitch + pitchSeperation * j;

			auto baseCircle = bgCircle;
			baseCircle.yaw = yaw;
			baseCircle.pitch = pitch;
			//gray는 항상 찍힌다
			dataList.push_back(baseCircle);

			if(j == 0) {
				//ai
				auto reinaWinData = reinaWinCircle;
				reinaWinData.yaw = yaw;
				reinaWinData.pitch = pitch;
				dataList.push_back(reinaWinData);
			} else {
				//player
				auto playerWinData = playerWinCircle;
				playerWinData.yaw = yaw;
				playerWinData.pitch = pitch;
				dataList.push_back(playerWinData);
			}
		}
	}

	hmdNode_ = Lib::smgr->addEmptySceneNode(cam);
	hmdNode_->grab();
	hmdNode_->setRotation(core::vector3df(0, 180, 0));

	for(auto data : dataList) {
		auto node = Lib::smgr->addEmptySceneNode(hmdNode_);
		node->setRotation(core::vector3df(data.pitch, data.yaw, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, data.tex);
		sprite->setSize(core::dimension2d<f32>(data.sizeX, data.sizeY));
		sprite->setPosition(core::vector3df(0, 0, data.radius));
		node->addChild(sprite);
		sprite->drop();

		if(data.tex == circleGreen) {
			playerScoreNodeList_.push_back(node);
		} else if(data.tex == circleRed) {
			aiScoreNodeList_.push_back(node);
		}
	}

	updateScoreNode(0, 0);
}

ScoreBoard::~ScoreBoard()
{
	hmdNode_->remove();
	hmdNode_->drop();
	hmdNode_ = nullptr;
}

void ScoreBoard::updateScoreNode(int playerScore, int aiScore)
{
	SR_ASSERT(playerScore >= 0 && playerScore <= (int)playerScoreNodeList_.size());
	SR_ASSERT(aiScore >= 0 && aiScore <= (int)aiScoreNodeList_.size());

	for(auto node : playerScoreNodeList_) {
		node->setVisible(false);
	}
	for(auto node : aiScoreNodeList_) {
		node->setVisible(false);
	}

	for(int i = 0 ; i < playerScore ; ++i) {
		playerScoreNodeList_[i]->setVisible(true);
	}
	for(int i = 0 ; i < aiScore ; ++i) {
		aiScoreNodeList_[i]->setVisible(true);
	}
}

void ScoreBoard::update()
{
	updateScoreNode(playerScore_, aiScore_);
}

bool ScoreBoard::isGameOver() const
{
	if(playerScore_ >= maxScore) {
		return true;
	}
	if(aiScore_ >= maxScore) {
		return true;
	}
	return false;
}

void ScoreBoard::reset()
{
	playerScore_ = 0;
	aiScore_ = 0;
	updateScoreNode(0, 0);
}