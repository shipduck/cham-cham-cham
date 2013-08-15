// Ŭnicode please 
#include "stdafx.h"
#include "main_sequence.h"
#include "irr/hmd_camera.h"
#include "base/lib.h"
#include "irr/debug_draw_manager.h"
#include "irr/sprite_scene_node.h"
#include "cvars/CVar.h"
#include "util/cvar_key.h"

using namespace std;
using namespace irr;

AbstractHMDCameraEventReceiver *receiver = nullptr;
irr::scene::IAnimatedMeshSceneNode *reinaNode = nullptr;

struct HMDInitData {
	float yaw;	//y축 기준. 좌우회전
	float pitch;	//x축 기준 회전. 위아래
	float sizeX;
	float sizeY;
	irr::video::ITexture *tex;
};

/*
class ScoreBoardSceneNode : public irr::scene::ISceneNode {
public:
private:
	int playerScore_;
	int aiScore_;
};
*/
MainSequence::MainSequence()
{
	auto cam = Lib::smgr->addCameraSceneNode();
	receiver = new HeadFreeCameraEventReceiver(cam, 0.1f, 0.1f);
	Lib::eventReceiver->attachReceiver(receiver);

	Lib::device->getCursorControl()->setVisible(false);

	irr::scene::IAnimatedMesh *reina = Lib::smgr->getMesh(res::modeldata::reira::G02T02_X);
	reinaNode = Lib::smgr->addAnimatedMeshSceneNode(reina);
	reinaNode->setPosition(irr::core::vector3df(0, 0, 0));
	reinaNode->setMaterialFlag(video::EMF_LIGHTING, false);
	reinaNode->setDebugDataVisible(scene::EDS_BBOX);


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
				baseCircle.yaw = 30 + 10 * i;
				baseCircle.pitch = -30 + 10 * j;
				baseCircle.tex = circleGray;
				baseCircle.sizeX = 2;
				baseCircle.sizeY = 2;
				dataList.push_back(baseCircle);
			}
		}
	}
	

	auto hmdNode = Lib::smgr->addEmptySceneNode(cam);
	hmdNode->setRotation(core::vector3df(0, 180, 0));

	for(auto data : dataList) {
		auto node = Lib::smgr->addEmptySceneNode(hmdNode);
		node->setRotation(core::vector3df(data.pitch, data.yaw, 0));
		auto sprite = new irr::scene::SpriteSceneNode(node, Lib::smgr, 0, data.tex);
		sprite->setSize(core::dimension2d<f32>(data.sizeX, data.sizeY));
		float radius = 30;
		sprite->setPosition(core::vector3df(0, 0, radius));
		node->addChild(sprite);
		sprite->drop();
	}
}

MainSequence::~MainSequence()
{
	Lib::eventReceiver->detachReceiver(receiver);
}

void MainSequence::update(int ms)
{
	//core::matrix4 m = core::IdentityMatrix;
	//g_debugDrawMgr->addAxis(m, 1000);

	//카메라 속도 관련
	float &moveSpeed = CVarUtils::GetCVarRef<float>(CVAR_GAME_CAM_MOVE_SPEED);
	float &rotateSpeed = CVarUtils::GetCVarRef<float>(CVAR_GAME_CAM_ROTATE_SPEED);
	receiver->moveSpeed = moveSpeed;
	receiver->rotateSpeed = rotateSpeed;

	//캐릭터 위치 설정
	float &posX = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_X);
	float &posY = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_Y);
	float &posZ = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_Z);
	reinaNode->setPosition(irr::core::vector3df(posX, posY, posZ));

	receiver->update(ms);
}