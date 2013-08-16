// Ŭnicode please 
#include "stdafx.h"
#include "main_sequence.h"
#include "irr/hmd_camera.h"
#include "base/lib.h"
#include "irr/debug_draw_manager.h"
#include "cvars/CVar.h"
#include "util/cvar_key.h"
#include "game/score_board.h"
#include "game/rock_paper_scissor.h"
#include "res.h"

using namespace std;
using namespace irr;

MainSequence::MainSequence()
	: receiver_(nullptr),
	reinaNode_(nullptr)
{
	//init camera
	auto cam = Lib::smgr->addCameraSceneNode();
	receiver_ = new HeadFreeCameraEventReceiver(cam, 0.1f, 0.1f);
	receiver_->enableCamMove = false;
	Lib::eventReceiver->attachReceiver(receiver_);
	Lib::device->getCursorControl()->setVisible(false);

	//init static
	initSkybox();

	//init dynamic
	irr::scene::IAnimatedMesh *reina = Lib::smgr->getMesh(res::modeldata::reira::G02T02_X);
	reinaNode_ = Lib::smgr->addAnimatedMeshSceneNode(reina);
	reinaNode_->setPosition(irr::core::vector3df(0, 0, 0));
	reinaNode_->setMaterialFlag(video::EMF_LIGHTING, false);
	reinaNode_->setDebugDataVisible(scene::EDS_BBOX);

	//init logic
	scoreBoard_.reset(new ScoreBoard(cam));
	rps_.reset(new RockPaperScissor(cam));	
}

MainSequence::~MainSequence()
{
	Lib::eventReceiver->detachReceiver(receiver_);
	receiver_ = nullptr;
}

irr::scene::ISceneNode *MainSequence::initSkybox()
{
	//skybox
	video::IVideoDriver* driver = Lib::device->getVideoDriver();
	scene::ISceneManager* smgr = Lib::device->getSceneManager();
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture(res::skybox::sky20::UPTRON_JPG),
		driver->getTexture(res::skybox::sky20::DOWNTRON_JPG),
		driver->getTexture(res::skybox::sky20::LEFTTRON_JPG),
		driver->getTexture(res::skybox::sky20::RIGHTTRON_JPG),
		driver->getTexture(res::skybox::sky20::FRONTTRON_JPG),
		driver->getTexture(res::skybox::sky20::BACKTRON_JPG));
	//scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("ext/irrlicht/media/skydome.jpg"),16,8,0.95f,2.0f);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	return skybox;
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
	rps_->update(ms);
	scoreBoard_->update();
}
