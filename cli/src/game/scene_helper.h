// Ŭnicode please 
#pragma once

class SceneHelper {
public:
	SceneHelper(irr::IrrlichtDevice *device);

	irr::scene::ISceneNode *initSky();
	irr::scene::ITerrainSceneNode* initTerrain();
private:
	irr::IrrlichtDevice *device;
};