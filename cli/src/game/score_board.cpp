// Ŭnicode please 
#include "stdafx.h"
#include "score_board.h"
#include "base/lib.h"
#include "irr/sprite_scene_node.h"
#include "res.h"

using namespace std;
using namespace irr;

struct HMDInitData {
	float yaw;	//y축 기준. 좌우회전
	float pitch;	//x축 기준 회전. 위아래
	float sizeX;
	float sizeY;
	irr::video::ITexture *tex;
};

ScoreBoard::ScoreBoard(irr::scene::ICameraSceneNode *cam)
	: playerScore_(0),
	aiScore_(0),
	hmdNode_(nullptr)
{
	//hmd
	auto reinaTex = Lib::driver->getTexture(res::texture::FACE_REINA_PNG);
	auto playerTex = Lib::driver->getTexture(res::texture::FACE_PLAYER_PNG);
	auto circleGreen = Lib::driver->getTexture(res::texture::CIRCLE_GREEN_PNG);
	auto circleRed = Lib::driver->getTexture(res::texture::CIRCLE_RED_PNG);
	auto circleGray = Lib::driver->getTexture(res::texture::CIRCLE_GRAY_PNG);

	vector<HMDInitData> dataList;
	{
		HMDInitData reina;
		reina.yaw = 20;
		reina.pitch = -30;
		reina.tex = reinaTex;
		reina.sizeX = 2;
		reina.sizeY = 2;
		dataList.push_back(reina);
	}
	{
		HMDInitData player;
		player.yaw = 20;
		player.pitch = -20;
		player.tex = playerTex;
		player.sizeX = 2;
		player.sizeY = 2;
		dataList.push_back(player);
	}
	{
		for(int i = 0 ; i < 3 ; i++) {
			for(int j = 0 ; j < 2 ; j++) {
				HMDInitData baseCircle;
				baseCircle.yaw = 30 + 10.0f * i;
				baseCircle.pitch = -30 + 10.0f * j;
				baseCircle.tex = circleGray;
				baseCircle.sizeX = 2;
				baseCircle.sizeY = 2;
				dataList.push_back(baseCircle);
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
		float radius = 30;
		sprite->setPosition(core::vector3df(0, 0, radius));
		node->addChild(sprite);
		sprite->drop();
	}
}

ScoreBoard::~ScoreBoard()
{
	hmdNode_->drop();
	hmdNode_ = nullptr;
}

void ScoreBoard::update()
{
}