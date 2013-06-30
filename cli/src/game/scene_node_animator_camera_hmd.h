// Ŭnicode please 
// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_SCENE_NODE_ANIMATOR_CAMERA_HMD_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_CAMERA_HMD_H_INCLUDED__

#include "ISceneNodeAnimator.h"
#include "vector2d.h"
#include "position2d.h"
#include "SKeyMap.h"
#include "irrArray.h"

class JoystickInputEvent;

//! Special scene node animator for FPS cameras
class SceneNodeAnimatorCameraHMD : public irr::scene::ISceneNodeAnimator
{
public:

	//! Constructor
	SceneNodeAnimatorCameraHMD(irr::IrrlichtDevice *dev, irr::gui::ICursorControl* cursorControl,
		irr::f32 rotateSpeed = 1.5f, irr::f32 moveSpeed = 0.5f, irr::f32 jumpSpeed=0.0f);

	//! Destructor
	virtual ~SceneNodeAnimatorCameraHMD();

	//! Animates the scene node, currently only works on cameras
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);

	//! Event receiver
	virtual bool OnEvent(const irr::SEvent& event);

	//! Returns the speed of movement in units per second
	virtual irr::f32 getMoveSpeed() const { return MoveSpeed; }

	//! Sets the speed of movement in units per second
	virtual void setMoveSpeed(irr::f32 moveSpeed) { MoveSpeed = moveSpeed; }

	//! Returns the rotation speed
	virtual irr::f32 getRotateSpeed() const { return RotateSpeed; }

	//! Set the rotation speed
	virtual void setRotateSpeed(irr::f32 rotateSpeed) { RotateSpeed = rotateSpeed; }

	virtual void setHeadTrackingValue(irr::f32 yaw, irr::f32 pitch, irr::f32 roll);

	//! This animator will receive events when attached to the active camera
	virtual bool isEventReceiverEnabled() const
	{
		return true;
	}

	//! Returns the type of this animator
	virtual irr::scene::ESCENE_NODE_ANIMATOR_TYPE getType() const
	{
		return irr::scene::ESNAT_CAMERA_FPS;
	}

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer once you're
	done with it. */
	virtual ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager=0);

	void rotateCamera(irr::scene::ISceneNode* node, irr::u32 ms);;
private:
	irr::gui::ICursorControl *CursorControl;

	irr::f32 MaxVerticalAngle;

	irr::f32 MoveSpeed;
	irr::f32 RotateSpeed;
	irr::f32 JumpSpeed;

	// head tracking sensor value
	irr::f32 Yaw;
	irr::f32 Pitch;
	irr::f32 Roll;

	irr::IrrlichtDevice* Device;
	JoystickInputEvent* joystick;
};

#endif // __C_SCENE_NODE_ANIMATOR_CAMERA_HMD_H_INCLUDED__

