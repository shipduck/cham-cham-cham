// Ŭnicode please 
#pragma once

#include "scene.h"
#include "game/scene_node_animator_camera_hmd.h"

class JoystickEventReceiver;

class SampleScene : public Scene {
public:
	SampleScene(irr::IrrlichtDevice *dev);
	virtual ~SampleScene();

	virtual void setUp();
	virtual void shutDown();

	virtual void update(int ms);
	virtual void draw();

private:
	irr::scene::IBillboardSceneNode *bill;
	irr::scene::ICameraSceneNode* camNode;
	SceneNodeAnimatorCameraHMD *hmdCam;

	// Remember which scene node is highlighted
    irr::scene::ISceneNode* highlightedSceneNode;
};