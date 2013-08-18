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
#include "game/rps_event.h"
#include "game/game_result.h"
#include "game/finger_direction_phase.h"
#include "game/finger_direction_event.h"
#include "res.h"
#include "sub_sequence.h"

using namespace std;
using namespace irr;

int resultShowRemain = 0;

MainSequence::MainSequence()
	: camReceiver_(nullptr),
	reinaNode_(nullptr)
{
	//init camera
	auto cam = Lib::smgr->addCameraSceneNode();
	camReceiver_ = new HeadFreeCameraEventReceiver(cam, 0.1f, 0.1f);
	camReceiver_->enableCamMove = false;
	Lib::eventReceiver->attachReceiver(camReceiver_);
	Lib::device->getCursorControl()->setVisible(false);

	//init static
	initSkybox();

	//init dynamic
	irr::scene::IAnimatedMesh *reina = Lib::smgr->getMesh(res::modeldata::reira::G02T02_X);
	//irr::scene::IAnimatedMesh *reina = Lib::smgr->getMesh(res::modeldata::reira::RSP_X);
	//irr::scene::IAnimatedMesh *reina = Lib::smgr->getMesh(res::modeldata::nagare::NAGARE_WALK_X);
	reinaNode_ = Lib::smgr->addAnimatedMeshSceneNode(reina);
    //reinaNode_->setJointMode(irr::scene::EJUOR_CONTROL);
    //reinaNode_->setTransitionTime(0.5);
	reinaNode_->setMaterialFlag(video::EMF_LIGHTING, false);
	reinaNode_->setDebugDataVisible(scene::EDS_BBOX);
    reinaNode_->setLoopMode(true);
    reinaNode_->setAnimationSpeed(5000.f);
    reinaNode_->setFrameLoop(reinaNode_->getStartFrame(), reinaNode_->getEndFrame());
    //reinaNode_->animateJoints();
    
    printf("%d ~ %d\n", reinaNode_->getStartFrame(), reinaNode_->getEndFrame());

	//init logic
	scoreBoard_.reset(new ScoreBoard(cam));

	//최초 상태는 가위바위보. 메뉴같은거 추가하면 그때 손보기
	subSequence_.reset(new RockPaperScissor(cam, scoreBoard_.get()));
}

MainSequence::~MainSequence()
{
	Lib::eventReceiver->detachReceiver(camReceiver_);
	camReceiver_ = nullptr;
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
	camReceiver_->moveSpeed = moveSpeed;
	camReceiver_->rotateSpeed = rotateSpeed;

	//캐릭터 위치 설정
	float &posX = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_X);
	float &posY = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_Y);
	float &posZ = CVarUtils::GetCVarRef<float>(CVAR_GAME_CHARACTER_POS_Z);
	reinaNode_->setPosition(irr::core::vector3df(posX, posY, posZ));


	camReceiver_->update(ms);
	scoreBoard_->update();

	//sub sequence 처리 + 다음 subsequence가 존재할경우, 분기
	auto next = subSequence_->update(ms);
	if(next.get() != nullptr) {
		subSequence_ = std::move(next);
	}
}
