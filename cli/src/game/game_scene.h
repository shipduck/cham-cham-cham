// Ŭnicode please 
#pragma once

#include "scene.h"
#include "game/scene_node_animator_camera_hmd.h"

class GameScene : public Scene {
public:
	GameScene(irr::IrrlichtDevice *dev);
	virtual ~GameScene();

	virtual void setUp();
	virtual void shutDown();

	virtual void update(int ms);
	virtual void draw();

	virtual void initCam();
	virtual void initSky();
private:
	irr::scene::ICameraSceneNode* camNode;
	SceneNodeAnimatorCameraHMD *hmdCam;
};