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

	void initCam();
	void initSky();
	irr::scene::ITerrainSceneNode* initTerrain();

	void initTargetableObject();

	irr::scene::ISceneNode *initColosseum();
	void initObstacleList();

private:
	irr::scene::IBillboardSceneNode *bill;
	irr::scene::ICameraSceneNode* camNode;
	SceneNodeAnimatorCameraHMD *hmdCam;
};