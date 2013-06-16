// Ŭnicode please 

#include "stdafx.h"
#include "JoystickEventReceiver.h"

#include "ISceneManager.h"

using namespace irr;
using namespace gui;
using namespace scene;

JoystickEventReceiver::JoystickEventReceiver()
{
	XMovement = 0.0f;
	YMovement = 0.0f;

	XView = 0.0f;
	YView = 0.0f;
}

bool JoystickEventReceiver::OnEvent(const SEvent& event)
{
	// The state of each connected joystick is sent to us
	// once every run() of the Irrlicht device.  Store the
	// state of the first joystick, ignoring other joysticks.
	// This is currently only supported on Windows and Linux.
	if (event.EventType == EET_JOYSTICK_INPUT_EVENT && event.JoystickEvent.Joystick == 0)
	{
		JoystickState = event.JoystickEvent;
	}

	if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown) {
		if (event.KeyInput.Key == KEY_ESCAPE)
		{
			Device->closeDevice();
			return true;
		}
	}
	
	return false;
}
const SEvent::SJoystickEvent& JoystickEventReceiver::getJoystickState(void) const
{
	return JoystickState;
}

void JoystickEventReceiver::update()
{
	XMovement = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
	YMovement = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;

	XView = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_R] / 32767.f;
	YView = (f32)JoystickState.Axis[SEvent::SJoystickEvent::AXIS_U] / 32767.f;

	YView = 0.0f; //temp

	const f32 DEAD_ZONE = 0.12f;

	if(fabs(XMovement) < DEAD_ZONE)
		XMovement = 0.0f;
	if(fabs(YMovement) < DEAD_ZONE)
		YMovement = 0.0f;

	if(fabs(XView) < DEAD_ZONE)
		XView = 0.0f;
	if(fabs(YView) < DEAD_ZONE)
		YView = 0.0f;

	const u16 povDegrees = JoystickState.POV / 100;

	if(povDegrees < 360)
	{
		if(povDegrees > 0 && povDegrees < 180)
			XMovement = 1.f;
		else if(povDegrees > 180)
			XMovement = -1.f;

		if(povDegrees > 90 && povDegrees < 270)
			YMovement = -1.f;
		else if(povDegrees > 270 || povDegrees < 90)
			YMovement = +1.f;
	}

	if(!core::equals(XMovement, 0.f) || !core::equals(YMovement, 0.f))
	{
		Moved = true;
	}
	else
	{
		Moved = false;
	}
}
void JoystickEventReceiver::rotateCamera(ISceneNode* node, u32 ms)
{
	if (!node || node->getType() != ESNT_CAMERA)
		return;

	ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);

	scene::ISceneManager * smgr = camera->getSceneManager();
	if(smgr && smgr->getActiveCamera() != camera)
		return;

	// update position
	core::vector3df pos = camera->getPosition();
	core::vector3df target = (camera->getTarget() - camera->getAbsolutePosition());
	
	core::matrix4 mat;
	mat.setRotationDegrees(core::vector3df(YView * 1.5f, XView * 1.5f, 0));
	mat.transformVect(target);

	// write translation
	camera->setPosition(pos);

	// write right target
	target += pos;
	camera->setTarget(target);
}

void JoystickEventReceiver::setDevice(IrrlichtDevice* device)
{
	Device = device;
}
void JoystickEventReceiver::setCursor(ICursorControl* cursor)
{
	Cursor = cursor;
}

f32 JoystickEventReceiver::getXMovement() const
{
	return XMovement;
}
f32 JoystickEventReceiver::getYMovement() const
{
	return YMovement;
}

f32 JoystickEventReceiver::getXView() const
{
	return XView;
}
f32 JoystickEventReceiver::getYView() const
{
	return YView;
}