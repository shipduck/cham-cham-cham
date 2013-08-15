// Ŭnicode please 
#include "stdafx.h"
#include "main_sequence.h"
#include "irr/hmd_camera.h"
#include "base/lib.h"
#include "irr/debug_draw_manager.h"
#include "cvars/CVar.h"
#include "util/cvar_key.h"
#include "game/score_board.h"
#include "res.h"

using namespace std;
using namespace irr;

MainSequence::MainSequence()
	: receiver_(nullptr),
	reinaNode_(nullptr)
{
	auto cam = Lib::smgr->addCameraSceneNode();
	receiver_ = new HeadFreeCameraEventReceiver(cam, 0.1f, 0.1f);
	Lib::eventReceiver->attachReceiver(receiver_);

	Lib::device->getCursorControl()->setVisible(false);

	irr::scene::IAnimatedMesh *reina = Lib::smgr->getMesh(res::modeldata::reira::G02T02_X);
	reinaNode_ = Lib::smgr->addAnimatedMeshSceneNode(reina);
	reinaNode_->setPosition(irr::core::vector3df(0, 0, 0));
	reinaNode_->setMaterialFlag(video::EMF_LIGHTING, false);
	reinaNode_->setDebugDataVisible(scene::EDS_BBOX);

	scoreBoard_.reset(new ScoreBoard(cam));
}

MainSequence::~MainSequence()
{
	Lib::eventReceiver->detachReceiver(receiver_);
}

void MainSequence::update(int ms)
{
	//core::matrix4 m = core::IdentityMatrix;
	//g_debugDrawMgr->addAxis(m, 1000);

	//카메라 속도 관련
	float &moveSpeed = CVarUtils::GetCVarRef<float>(CVAR_GAME_CAM_MOVE_SPEED);
	float &rotateSpeed = CVarUtils::GetCVarRef<float>(CVAR_GAME_CAM_ROTATE_SPEED);
	receiver_->moveSpeed = moveSpeed;
	receiver_->rotateSpeed = rotateSpeed;

	//캐릭터 위치 설정
	float &posX = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_X);
	float &posY = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_Y);
	float &posZ = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_Z);
	reinaNode_->setPosition(irr::core::vector3df(posX, posY, posZ));

	receiver_->update(ms);
	scoreBoard_->update();
}
