// Ŭnicode please 
#include "stdafx.h"
#include "game_scene.h"
#include "scene_helper.h"

using namespace irr;
using namespace video;
using namespace gui;
using namespace core;
using namespace scene;

enum {
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


GameScene::GameScene(irr::IrrlichtDevice *dev)
	: Scene(dev), 
	hmdCam(nullptr), 
	camNode(nullptr), 
	bill(nullptr)
{
}

GameScene::~GameScene()
{
}

void GameScene::setUp()
{
	Scene::setUp();

	Device->setWindowCaption(L"Game Scene");

	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();

	guienv->addStaticText(L"Project::Dokuro", rect<s32>(10,10,260,22), true);

	initSky();
	initCam();

	//충돌 테스트용 객체를 적절히 배치
	{
		SMaterial material;
		material.Lighting = false;
		material.AmbientColor = SColor(255, 255, 0, 0);

		for(float x = -1010 ; x <= 1000 ; x += 100) {
			for(float z = -1010 ; z <= 1000 ; z += 100) {
				auto node = smgr->addCubeSceneNode();
				node->setPosition(vector3df(x, 0, z));
				node->getMaterial(0) = material;

				auto selector = smgr->createTriangleSelector(node->getMesh(), node);
				node->setTriangleSelector(selector);
				selector->drop();
			}
		}
	}

	//선택 테스트용 객체 배치
	{
		SMaterial material;
		material.AmbientColor = SColor(255, 255, 0, 0);

		for(float x = -1050 ; x <= 1000 ; x += 200) {
			for(float z = -1050 ; z <= 1000 ; z += 200) {
				auto node = smgr->addCubeSceneNode();

				// id이용해서 특정 노드가 선택가능/불가능을 확인하도록 임시코딩
				node->setID(IDFlag_IsPickable);
				node->setPosition(vector3df(x, 0, z));
				node->getMaterial(0) = material;

				auto selector = smgr->createTriangleSelector(node->getMesh(), node);
				node->setTriangleSelector(selector);
				selector->drop();
			}
		}
	}

	//선택한거 표시용 billboard
	{
		bill = smgr->addBillboardSceneNode();
		bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
		bill->setMaterialTexture(0, driver->getTexture("ext/irrlicht/media/particle.bmp"));
		bill->setMaterialFlag(video::EMF_LIGHTING, false);
		bill->setMaterialFlag(video::EMF_ZBUFFER, false);
		bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
		bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it
	}
}

void GameScene::initCam()
{
	ISceneManager* smgr = Device->getSceneManager();

	// Create Camera
	camNode = smgr->addCameraSceneNode();
	auto cursorControl = Device->getCursorControl();
	hmdCam = new SceneNodeAnimatorCameraHMD(Device, cursorControl, 0.3f, 0.05f, 0.0f);

	camNode->addAnimator(hmdCam);
	//TODO 카메라 위치 잡기
	camNode->setPosition(core::vector3df(0, 0, 0));
	camNode->setTarget(core::vector3df(0, 0, 1));
	camNode->setFarValue(1000.0f);
}

void GameScene::initSky()
{
	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();

	// create skybox and skydome
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	//scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
	//	driver->getTexture("ext/irrlicht/media/irrlicht2_up.jpg"),
	//	driver->getTexture("ext/irrlicht/media/irrlicht2_dn.jpg"),
	//	driver->getTexture("ext/irrlicht/media/irrlicht2_lf.jpg"),
	//	driver->getTexture("ext/irrlicht/media/irrlicht2_rt.jpg"),
	//	driver->getTexture("ext/irrlicht/media/irrlicht2_ft.jpg"),
	//	driver->getTexture("ext/irrlicht/media/irrlicht2_bk.jpg"));
	scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode(driver->getTexture("ext/irrlicht/media/skydome.jpg"),16,8,0.95f,2.0f);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
}


void GameScene::shutDown()
{
	Scene::shutDown();

	//delete(hmdCam);
	//hmdCam = nullptr;
}
void GameScene::draw()
{
}

void GameScene::update(int ms)
{
	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();
	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();


	// All intersections in this example are done with a ray cast out from the camera to
    // a distance of 1000.  You can easily modify this to check (e.g.) a bullet
    // trajectory or a sword's position, or create a ray from a mouse click position using
    // ISceneCollisionManager::getRayFromScreenCoordinates()
    core::line3d<f32> ray;
	ray.start = camNode->getPosition();
    ray.end = ray.start + (camNode->getTarget() - ray.start).normalize() * 1000.0f;

    // Tracks the current intersection point with the level or a mesh
    core::vector3df intersection;
    // Used to show with triangle has been hit
    core::triangle3df hitTriangle;

	// This call is all you need to perform ray/triangle collision on every scene node
    // that has a triangle selector, including the Quake level mesh.  It finds the nearest
    // collision point/triangle, and returns the scene node containing that point.
    // Irrlicht provides other types of selection, including ray/triangle selector,
    // ray/box and ellipse/triangle selector, plus associated helpers.
    // See the methods of ISceneCollisionManager
    scene::ISceneNode * selectedSceneNode =
        collMan->getSceneNodeAndCollisionPointFromRay(
                ray,
                intersection, // This will be the position of the collision
                hitTriangle, // This will be the triangle hit in the collision
                IDFlag_IsPickable, // This ensures that only nodes that we have
                        // set up to be pickable are considered
                0); // Check the entire scene (this is actually the implicit default)

    // If the ray hit anything, move the billboard to the collision position
    // and draw the triangle that was hit.
	bill->setVisible(false);
    if(selectedSceneNode && selectedSceneNode->getID() == IDFlag_IsPickable) {
		bill->setPosition(intersection);
		bill->setVisible(true);
	}
}
