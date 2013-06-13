// Ŭnicode please 
#pragma once

#include "scene.h"
#include "hmd_display.h"
#include "scene_node_animator_camera_hmd.h"

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

class MyEventReceiver : public irr::IEventReceiver {
public:
	bool OnEvent(const irr::SEvent& event);
	irr::IrrlichtDevice* device;
	irr::gui::ICursorControl* cursor;
};

class SampleScene : public Scene {
public:
	SampleScene(irr::IrrlichtDevice *dev);
	virtual ~SampleScene();

	virtual void update(int ms);
	virtual void setUp();
	virtual void shutDown();

private:
	MyEventReceiver receiver;

	irr::scene::IBillboardSceneNode *bill;
	irr::scene::ICameraSceneNode* camNode;
	irr::scene::CSceneNodeAnimatorCameraHMD *hmdCam;

	HMDDisplay display;

	// Remember which scene node is highlighted
    irr::scene::ISceneNode* highlightedSceneNode;
};