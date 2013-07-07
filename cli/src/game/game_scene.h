// Ŭnicode please 
#pragma once

#include "scene.h"

struct MoveEvent;
struct LookEvent;
class GameEventReceiver;

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

	void updateMoveEvent(int ms, const MoveEvent &evt);
	void updateLookEvent(int ms, const LookEvent &evt);

private:
	irr::scene::IBillboardSceneNode *bill;
	irr::scene::ICameraSceneNode* camNode;

	GameEventReceiver *eventReceiver_;

	irr::f32 MoveSpeed;
	irr::f32 RotateSpeed;
	irr::f32 JumpSpeed;
};