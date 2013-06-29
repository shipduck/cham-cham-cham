// Ŭnicode please 
#include "stdafx.h"
#include "game_scene.h"
#include "scene_helper.h"
#include "util/debug_draw_manager.h"

using namespace irr;
using namespace video;
using namespace gui;
using namespace core;
using namespace scene;

enum {
    ID_IsNotPickable = 0,
    IDFlag_Wall = 1 << 0,
	IDFlag_IsPickable = 1 << 1,
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

	initSky();
	initCam();
	//initTargetableObject();
	initColosseum();
	initObstacleList();
	
	{
		auto terrain = initTerrain();
	
		// create triangle selector for the terrain	
		auto selector = smgr->createTerrainTriangleSelector(terrain, 0);
		terrain->setTriangleSelector(selector);

		// create collision response animator and attach it to the camera
		scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camNode, core::vector3df(10, 10, 10),
			core::vector3df(0, -10, 0),
			core::vector3df(0, 10, 0));
		camNode->addAnimator(anim);

		anim->drop();
		selector->drop();
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
	hmdCam = new SceneNodeAnimatorCameraHMD(Device, cursorControl, 0.3f, 0.10f, 0.0f);

	camNode->addAnimator(hmdCam);
	camNode->setPosition(core::vector3df(0, 100, 0));
	camNode->setTarget(core::vector3df(0, 0, 1));
	camNode->setFarValue(1000.0f);
	hmdCam->drop();
	
}

void GameScene::initTargetableObject()
{
	//선택 테스트용 객체 배치
	ISceneManager* smgr = Device->getSceneManager();
	
	SMaterial material;
	material.AmbientColor = SColor(255, 255, 0, 0);

	for(float x = 50 ; x <= 1000 ; x += 200) {
		for(float z = 50 ; z <= 1000 ; z += 200) {
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

irr::scene::ISceneNode *GameScene::initColosseum()
{
	ISceneManager* smgr = Device->getSceneManager();
	IVideoDriver* driver = Device->getVideoDriver();

	float radius = 500;
	const int numSegment = 32;
	float rad = 2 * PI / numSegment;
	float wallHeightHigh = 60;
	float wallHeightLow = 50;

	struct WallSegment {
		std::array<float, 2> P1;
		std::array<float, 2> P2;
		float rot;
	};

	std::array<WallSegment, numSegment> segmentList;
	for(int i = 0 ; i < numSegment ; i++) {
		int a = i;
		int b = (i + 1) % numSegment;

		WallSegment &seg = segmentList[i];
		seg.P1[0] = radius * cos(rad * a);
		seg.P1[1] = radius * sin(rad * a);
		seg.P2[0] = radius * cos(rad * b);
		seg.P2[1] = radius * sin(rad * b);
		seg.rot = (a + b) * 0.5f * rad;
	}

	//create scene node
	auto root = smgr->addEmptySceneNode();

	SMaterial material;
	material.setTexture(0, driver->getTexture("ext/irrlicht/media/wall.bmp"));
	material.Lighting = false;

	for(int i = 0 ; i < numSegment ; ++i) {
		const WallSegment &seg = segmentList[i];

		float x = (seg.P1[0] + seg.P2[0]) * 0.5f;
		float z = (seg.P1[1] + seg.P2[1]) * 0.5f;

		auto node = smgr->addCubeSceneNode(10.0f, root);
		node->setPosition(vector3df(x, 0, z));
		float deg = irr::core::radToDeg(seg.rot);
		node->setRotation(vector3df(0, -deg, 0));

		node->getMaterial(0) = material;

		//높이를 다르게 하면 벽이 톱니바퀴같은 느낌이 될거다
		float height = wallHeightHigh;
		if(i % 2 == 0) {
			height = wallHeightLow;
		}
		float wallWidth = rad * radius * 0.1f;
		node->setScale(vector3df(0.1f, height, wallWidth));

		//벽은 못지나간다
		auto selector = smgr->createTriangleSelector(node->getMesh(), node);
		node->setTriangleSelector(selector);
				
		// create collision response animator and attach it to the camera
		scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camNode, core::vector3df(10, 10, 10),
			core::vector3df(0, 0, 0),
			core::vector3df(0, 0, 0));
				
		camNode->addAnimator(anim);
		anim->drop();
		selector->drop();
	}
	return root;
}

/*
충돌 테스트용 객체를 적절히 배치
이런것도 지원한다 라는걸 보여주는것이 목표니까 간단하게 구현
진짜 객체로 감싸는건 나중에
*/
void GameScene::initObstacleList()
{
	ISceneManager* smgr = Device->getSceneManager();
	IVideoDriver* driver = Device->getVideoDriver();

	SMaterial material;
	material.Lighting = false;
	material.AmbientColor = SColor(255, 255, 0, 0);
	material.setTexture(0, driver->getTexture("ext/irrlicht/media/wall.jpg"));

	std::vector<vector3df> posList;
	posList.push_back(vector3df(-100, 0, 100));
	posList.push_back(vector3df(100, 0, 100));
	posList.push_back(vector3df(100, 0, -100));
	posList.push_back(vector3df(-100, 0, -100));

	for(auto pos : posList) {
		auto node = smgr->addCubeSceneNode();
		node->setPosition(pos);
		node->getMaterial(0) = material;

		node->setScale(vector3df(2, 5, 2));

		auto selector = smgr->createTriangleSelector(node->getMesh(), node);
		node->setTriangleSelector(selector);
				
		scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camNode, core::vector3df(10, 10, 10),
			core::vector3df(0, 0, 0),
			core::vector3df(0, 0, 0));
				
		camNode->addAnimator(anim);
		anim->drop();
		selector->drop();
	}
}

irr::scene::ITerrainSceneNode* GameScene::initTerrain()
{
	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();

	// 하이트맵의 가운데 == 0,0,0이 되도록 만든다. 게임로직을 시작할떄
	// 중심에 모아놔야 디버깅하기 쉬우니까
	float scaleVal = 20.0f;
	auto position = core::vector3df(0.f, 0.0f, 0.f);
	auto rotation = core::vector3df(0.f, 0.f, 0.f);
	auto scale = core::vector3df(scaleVal, 1.0f, scaleVal);
	auto vertexColor = video::SColor(255, 255, 255, 255);	
	s32 maxLOD = 5;
	E_TERRAIN_PATCH_SIZE patchSize = scene::ETPS_17;
	s32 smoothFactor = 4;
	int nodeId = 0;

	//지형으로 필요한건 완전평면 하나이다
	//그래서 검정색=0 으로 구성된 height map를 사용했다
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"res/terrain-heightmap.png",
		0,
		nodeId,
		position,
		rotation,
		scale,
		vertexColor,
		maxLOD,
		patchSize,
		smoothFactor
		);
	SR_ASSERT(terrain != nullptr);

	auto terrainCenter = terrain->getTerrainCenter();
	terrain->setPosition(-terrainCenter);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0,
			driver->getTexture("ext/irrlicht/media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1,
			driver->getTexture("ext/irrlicht/media/detailmap3.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(100.0f, 100.0f);

	return terrain;
}
void GameScene::initSky()
{
	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();

	// create skybox and skydome
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode(driver->getTexture("ext/irrlicht/media/skydome.jpg"),16,8,0.95f,2.0f);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
}


void GameScene::shutDown()
{
	Scene::shutDown();
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

	//카메라 현재 위치 찍기
	using boost::wformat;
	auto camPos = camNode->getPosition();
	auto camPosMsg = (wformat(L"CamPos : %.2f, %.2f, %.2f") % camPos.X % camPos.Y % camPos.Z).str();
	SColor white(255, 255, 255, 255);
	gDebugDrawMgr->addString(vector2di(0, 0), camPosMsg, white);
}
