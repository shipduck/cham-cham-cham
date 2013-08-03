// Ŭnicode please 
#pragma once

#include "sequence.h"

struct MoveEvent;
struct LookEvent;
class GameEventReceiver;

class GameSequence : public Sequence {
public:
	GameSequence();
	virtual ~GameSequence();

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