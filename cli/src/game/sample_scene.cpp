// Ŭnicode please 
#include "stdafx.h"
#include "sample_scene.h"
#include "scene_helper.h"
#include "scene_node_animator_camera_hmd.h"
#include "JoystickEventReceiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

ICameraSceneNode* initCamera(IrrlichtDevice *device, ISceneManager *smgr, scene::ITerrainSceneNode *terrain)
{
	ICameraSceneNode* camNode = smgr->addCameraSceneNode();
	auto cursorControl = device->getCursorControl();
	CSceneNodeAnimatorCameraHMD* hmdCam = new CSceneNodeAnimatorCameraHMD(cursorControl);
	camNode->addAnimator(hmdCam);
	camNode->setID(ID_IsNotPickable);
	camNode->setPosition(core::vector3df(2700*2,255*2 + 100,2600*2));
	camNode->setTarget(core::vector3df(2397*2,343*2,2700*2));
	camNode->setFarValue(42000.0f);
	
	// create triangle selector for the terrain	
	auto selector = smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, camNode, core::vector3df(60,100,60),
		core::vector3df(0,-10,0),
		core::vector3df(0,50,0));
	selector->drop();
	camNode->addAnimator(anim);
	anim->drop();
	
	return camNode;
}


SampleScene::SampleScene(irr::IrrlichtDevice *dev)
	: Scene(dev),
	bill(nullptr),
	camNode(nullptr),
	hmdCam(nullptr),
	highlightedSceneNode(nullptr),
	mouseSpeed(40.0f),
	walkSpeed(500.0f)
{
}

SampleScene::~SampleScene()
{
}

void SampleScene::setUp()
{
	Scene::setUp();

	Receiver = new JoystickEventReceiver;
	Device->setEventReceiver(Receiver);
	Device->activateJoysticks(JoystickInfo);

	Device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	Receiver->setDevice(Device);
	Receiver->setCursor(Device->getCursorControl());

	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);

	SceneHelper sceneHelper(Device);

	display.setUp(Device);

	// Create world
	sceneHelper.initSky();

	scene::ITerrainSceneNode* terrain = sceneHelper.initTerrain();


	// Create Camera
	camNode = initCamera(Device, smgr, terrain);
	auto camAnim = *camNode->getAnimators().begin();
	hmdCam = static_cast<CSceneNodeAnimatorCameraHMD*>(camAnim);

	// load a dwarf
	IAnimatedMesh* dwarf = smgr->getMesh("ext/irrlicht/media/dwarf.x");
	IAnimatedMeshSceneNode* dwarfNode = smgr->addAnimatedMeshSceneNode(dwarf, 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	dwarfNode->setPosition(core::vector3df(2700*2 + 100,255*2 + 100,2600*2));
	{
		// Now create a triangle selector for it.  The selector will know that it
		// is associated with an animated node, and will update itself as necessary.
		auto selector = smgr->createTriangleSelector(dwarfNode);
		dwarfNode->setTriangleSelector(selector);
		selector->drop(); // We're done with this selector, so drop it now.
	}

	// Add the billboard.
    bill = smgr->addBillboardSceneNode();
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    bill->setMaterialTexture(0, driver->getTexture("ext/irrlicht/media/particle.bmp"));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialFlag(video::EMF_ZBUFFER, false);
    bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
    bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it
	
	Device->getCursorControl()->setVisible(false);
}

void SampleScene::shutDown()
{
	Scene::shutDown();

	delete(hmdCam);
	hmdCam = nullptr;
}

void SampleScene::update(int ms)
{
	core::vector3df nodePosition = camNode->getPosition();

	if(JoystickInfo.size() > 0)
	{
		Receiver->update();

		f32 moveHorizontal = Receiver->getXMovement(); // Range is -1.f for full left to +1.f for full right
		f32 moveVertical = Receiver->getYMovement(); // -1.f for full down to +1.f for full up.

		float verticalMoveDelta = moveVertical * walkSpeed * ms / 1000.0f;
		float horizontalMoveDelta = moveHorizontal * walkSpeed * ms / 1000.0f;

		vector3df forward = camNode->getTarget() - camNode->getPosition();
		f32 distance = sqrt(pow(forward.Z,2) + pow(forward.X,2));

		float sine = forward.Z / distance;
		float cosine = forward.X / distance;

		// Rotate -90
		float sineTemp = sine;
		sine = -cosine;
		cosine = sineTemp;

		if(!core::equals(moveHorizontal, 0.f) || !core::equals(moveVertical, 0.f))
		{
			vector3df moveVector;
		
			moveVector.X  = horizontalMoveDelta * cosine - verticalMoveDelta * sine;
			moveVector.Z = horizontalMoveDelta * sine + verticalMoveDelta * cosine;

			nodePosition.X += moveVector.X;
			nodePosition.Z += moveVector.Z;

			printf("sine : %f, cosine : %f\n", sine, cosine);
		}

		camNode->setPosition(nodePosition);

		Receiver->rotateCamera(camNode, ms);
	}

	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();
	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();


	driver->beginScene(true,true,SColor(0,100,100,100));

	display.NormalDrawAll(smgr);
	//display.StereoDrawAll(smgr);

	guienv->drawAll();

		
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
    if(selectedSceneNode)
    {
        bill->setPosition(intersection);

		SMaterial material;
		material.Wireframe = true;
		material.Lighting = false;
		material.setTexture(0, 0);

        // We need to reset the transform before doing our own rendering.
        driver->setTransform(video::ETS_WORLD, core::matrix4());
		driver->setMaterial(material);
        driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));

        // We can check the flags for the scene node that was hit to see if it should be
        // highlighted. The animated nodes can be highlighted, but not the Quake level mesh
        if((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
        {
            highlightedSceneNode = selectedSceneNode;

            // Highlighting in this case means turning lighting OFF for this node,
            // which means that it will be drawn with full brightness.
            highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
        }
    }

	// end scene
	driver->endScene();	
}