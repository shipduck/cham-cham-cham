// Ŭnicode please 
#include "scene_helper.h"
#include "ISceneManager.h"
#include "ITerrainSceneNode.h"
#include "IrrlichtDevice.h"

using namespace irr;
using namespace irr::video;
using namespace irr::scene;

int foo()
{
	return 10;
}

CSceneHelper::CSceneHelper(irr::IrrlichtDevice *device)
	: device(device)
{
}

scene::ISceneNode *CSceneHelper::initSky()
{
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	// create skybox and skydome
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture("ext/irrlicht/media/irrlicht2_up.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_dn.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_lf.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_rt.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_ft.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_bk.jpg"));
	//scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("ext/irrlicht/media/skydome.jpg"),16,8,0.95f,2.0f);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	return skybox;
}

scene::ITerrainSceneNode *CSceneHelper::initTerrain()
{
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	// add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"ext/irrlicht/media/terrain-heightmap.bmp",
		0,					// parent node
		0,					// node id
		core::vector3df(0.f, 0.f, 0.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(40.f, 4.4f, 40.f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,					// maxLOD
		scene::ETPS_17,				// patchSize
		4					// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0,
			driver->getTexture("ext/irrlicht/media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1,
			driver->getTexture("ext/irrlicht/media/detailmap3.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(1.0f, 20.0f);

	return terrain;
}