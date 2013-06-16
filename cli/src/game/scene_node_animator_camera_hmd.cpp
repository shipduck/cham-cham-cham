// Copyright (C) 2002-2012 Nikolausf Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#include "stdafx.h"

#include "scene_node_animator_camera_hmd.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "Keycodes.h"
#include "ICursorControl.h"
#include "ICameraSceneNode.h"
#include "ISceneNodeAnimatorCollisionResponse.h"


using namespace irr;
using namespace core;
using namespace scene;

//! constructor
SceneNodeAnimatorCameraHMD::SceneNodeAnimatorCameraHMD(IrrlichtDevice *dev, gui::ICursorControl* cursorControl,
		f32 rotateSpeed, f32 moveSpeed, f32 jumpSpeed)
: Device(dev), CursorControl(cursorControl), MaxVerticalAngle(88.0f),
	MoveSpeed(moveSpeed), RotateSpeed(rotateSpeed), JumpSpeed(jumpSpeed),
	Yaw(0), Pitch(0), Roll(0),
	XMovement(0.0f),
	YMovement(0.0f),
	XView(0.0f),
	YView(0.0f),
	Moved(false)
{
	#ifdef _DEBUG
	setDebugName("CameraSceneNodeAnimatorHMD");
	#endif

	if (CursorControl) {
		CursorControl->grab();
	}

	Device->activateJoysticks(JoystickInfo);
}


//! destructor
SceneNodeAnimatorCameraHMD::~SceneNodeAnimatorCameraHMD()
{
	if (CursorControl) {
		CursorControl->drop();
	}
}


//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addFPSCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever.
bool SceneNodeAnimatorCameraHMD::OnEvent(const SEvent& evt)
{
	// The state of each connected joystick is sent to us
	// once every run() of the Irrlicht device.  Store the
	// state of the first joystick, ignoring other joysticks.
	// This is currently only supported on Windows and Linux.
	if (evt.EventType == EET_JOYSTICK_INPUT_EVENT && evt.JoystickEvent.Joystick == 0) {
		JoystickState = evt.JoystickEvent;
	}

	if (evt.EventType == EET_KEY_INPUT_EVENT && evt.KeyInput.PressedDown) {
		if (evt.KeyInput.Key == KEY_ESCAPE)
		{
			SR_ASSERT(Device != nullptr);
			Device->closeDevice();
			return true;
		}
	}
	
	return false;
}


void SceneNodeAnimatorCameraHMD::animateNode(ISceneNode* node, u32 timeMs)
{
	if (!node || node->getType() != ESNT_CAMERA) {
		return;
	}

	ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);
	scene::ISceneManager * smgr = camera->getSceneManager();
	if(smgr && smgr->getActiveCamera() != camera) {
		return;
	}

	if(JoystickInfo.size() == 0) {
		return;
	}
	
	// 기본 변수 초기화
	XMovement = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
	YMovement = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;

	XView = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_R] / 32767.f;
	YView = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_U] / 32767.f;

	YView = 0.0f; //temp

	const f32 DEAD_ZONE = 0.12f;

	if(fabs(XMovement) < DEAD_ZONE) {
		XMovement = 0.0f;
	}
	if(fabs(YMovement) < DEAD_ZONE) {
		YMovement = 0.0f;
	}

	if(fabs(XView) < DEAD_ZONE) {
		XView = 0.0f;
	}
	if(fabs(YView) < DEAD_ZONE) {
		YView = 0.0f;
	}

	const u16 povDegrees = JoystickState.POV / 100;

	if(povDegrees < 360) {
		if(povDegrees > 0 && povDegrees < 180) {
			XMovement = 1.f;
		} else if(povDegrees > 180) {
			XMovement = -1.f;
		}
		if(povDegrees > 90 && povDegrees < 270) {
			YMovement = -1.f;
		} else if(povDegrees > 270 || povDegrees < 90) {
			YMovement = +1.f;
		}
	}

	if(!core::equals(XMovement, 0.f) || !core::equals(YMovement, 0.f)) {
		Moved = true;
	} else {
		Moved = false;
	}

	////////////////////////////////////////

	f32 moveHorizontal = getXMovement(); // Range is -1.f for full left to +1.f for full right
	f32 moveVertical = getYMovement(); // -1.f for full down to +1.f for full up.

	float verticalMoveDelta = moveVertical * MoveSpeed * timeMs / 1000.0f;
	float horizontalMoveDelta = moveHorizontal * MoveSpeed * timeMs / 1000.0f;

	vector3df forward = camera->getTarget() - camera->getPosition();
	f32 distance = sqrt(pow(forward.Z,2) + pow(forward.X,2));

	float sine = forward.Z / distance;
	float cosine = forward.X / distance;

	// Rotate -90
	float sineTemp = sine;
	sine = -cosine;
	cosine = sineTemp;

	// update position
	vector3df pos = camera->getPosition();
	if(!core::equals(moveHorizontal, 0.f) || !core::equals(moveVertical, 0.f)) {
		vector3df moveVector;
		
		moveVector.X  = horizontalMoveDelta * cosine - verticalMoveDelta * sine;
		moveVector.Z = horizontalMoveDelta * sine + verticalMoveDelta * cosine;

		pos.X += moveVector.X;
		pos.Z += moveVector.Z;

		//printf("sine : %f, cosine : %f\n", sine, cosine);
	}
	// write translation
	camera->setPosition(pos);
	
	core::vector3df target = (camera->getTarget() - camera->getAbsolutePosition());

	core::matrix4 mat;
	mat.setRotationDegrees(core::vector3df(YView * RotateSpeed, XView * RotateSpeed, 0));
	mat.transformVect(target);

	// write right target
	target += pos;
	camera->setTarget(target);
}

void SceneNodeAnimatorCameraHMD::setHeadTrackingValue(f32 yaw, f32 pitch, f32 roll)
{
	Yaw = yaw;
	Pitch = pitch;
	Roll = roll;
	//printf("%f, %f, %f\n", yaw, pitch, roll);
}


ISceneNodeAnimator* SceneNodeAnimatorCameraHMD::createClone(ISceneNode* node, ISceneManager* newManager)
{
	SceneNodeAnimatorCameraHMD * newAnimator =
		new SceneNodeAnimatorCameraHMD(Device, CursorControl,	RotateSpeed, MoveSpeed, JumpSpeed);
	return newAnimator;
}

