// Ŭnicode please 
#pragma once

int foo();

namespace irr {;
class IrrlichtDevice;
namespace scene {;
class ISceneNode;
class ITerrainSceneNode;
}
}

class CSceneHelper {
public:
	CSceneHelper(irr::IrrlichtDevice *device);

	irr::scene::ISceneNode *initSky();
	irr::scene::ITerrainSceneNode* initTerrain();
private:
	irr::IrrlichtDevice *device;
};