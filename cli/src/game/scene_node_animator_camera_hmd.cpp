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

#include "joystick_input_event.h"

using namespace irr;
using namespace core;
using namespace scene;

//! constructor
SceneNodeAnimatorCameraHMD::SceneNodeAnimatorCameraHMD(IrrlichtDevice *dev, gui::ICursorControl* cursorControl,
		f32 rotateSpeed, f32 moveSpeed, f32 jumpSpeed)
: Device(dev), CursorControl(cursorControl), MaxVerticalAngle(88.0f),
	MoveSpeed(moveSpeed), RotateSpeed(rotateSpeed), JumpSpeed(jumpSpeed),
	Yaw(0), Pitch(0), Roll(0)
{
	#ifdef _DEBUG
	setDebugName("CameraSceneNodeAnimatorHMD");
	#endif

	joystick = new JoystickInputEvent;

	if (CursorControl) {
		CursorControl->grab();
	}

	Device->setEventReceiver(joystick);
	joystick->setDevice(Device);
	joystick->activateJoystickEvent();
}


//! destructor
SceneNodeAnimatorCameraHMD::~SceneNodeAnimatorCameraHMD()
{
	if (CursorControl) {
		CursorControl->drop();
	}

	delete joystick;
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

	joystick->update();
	const auto& JoystickInfo = joystick->getJoystickInfo();

	if(JoystickInfo.size() == 0) {
		return;
	}
	
	////////////////////////////////////////

	f32 moveHorizontal = joystick->getHorizontalMovement(); // Range is -1.f for full left to +1.f for full right
	f32 moveVertical = joystick->getVerticalMovement(); // -1.f for full down to +1.f for full up.
	f32 rotateHoriziontal = joystick->getLeftRightRotation();
	f32 rotateVertical = joystick->getUpDownRotation();

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
	mat.setRotationDegrees(core::vector3df(rotateVertical * RotateSpeed, rotateHoriziontal * RotateSpeed, 0));
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

