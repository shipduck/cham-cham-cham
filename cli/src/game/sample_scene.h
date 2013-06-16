// Ŭnicode please 
#pragma once

#include "scene.h"
#include "util/hmd_display.h"
#include "game/scene_node_animator_camera_hmd.h"

class JoystickEventReceiver;

enum
{
    // I use this ISceneNode ID to indicate a scene node that is
    // not pickable by getSceneNodeAndCollisionPointFromRay()
    ID_IsNotPickable = 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be picked by ray selection.
    IDFlag_IsPickable = 1 << 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be highlighted.  In this example, the
    // homonids can be highlighted, but the level mesh can't.
    IDFlag_IsHighlightable = 1 << 1
};

class SampleScene : public Scene {
public:
	SampleScene(irr::IrrlichtDevice *dev);
	virtual ~SampleScene();

	virtual void update(int ms);
	virtual void setUp();
	virtual void shutDown();

private:
	irr::scene::IBillboardSceneNode *bill;
	irr::scene::ICameraSceneNode* camNode;
	irr::scene::CSceneNodeAnimatorCameraHMD *hmdCam;

	JoystickEventReceiver *Receiver;
	irr::core::array<irr::SJoystickInfo> JoystickInfo;

	HMDDisplay display;

	float mouseSpeed;
	float walkSpeed;

	// Remember which scene node is highlighted
    irr::scene::ISceneNode* highlightedSceneNode;
};