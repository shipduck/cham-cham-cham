// Ŭnicode please 
#include "stdafx.h"
#include "main_sequence.h"
#include "irr/hmd_camera.h"
#include "base/lib.h"
#include "irr/debug_draw_manager.h"
#include "cvars/CVar.h"
#include "util/cvar_key.h"
#include "game/score_board.h"

using namespace std;
using namespace irr;

AbstractHMDCameraEventReceiver *receiver = nullptr;
irr::scene::IAnimatedMeshSceneNode *reinaNode = nullptr;
std::unique_ptr<ScoreBoard> scoreBoard;


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

	scoreBoard.reset(new ScoreBoard(cam));
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
