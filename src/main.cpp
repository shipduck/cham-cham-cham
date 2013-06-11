// Ŭnicode please 
#include <irrlicht.h>
#include "hmd_display.h"
#include "head_tracking.h"
#include "scene_helper.h"
#include "scene_node_animator_camera_hmd.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// Configuration

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 800;
bool fullscreen = false;
bool vsync = true;
float mouseSpeed = 40.0f;
float walkSpeed = 3.0f;

class MyEventReceiver : public IEventReceiver {
public:
	bool OnEvent(const SEvent& event)
	{   
		if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown) {
			if (event.KeyInput.Key == KEY_ESCAPE)
			{
				device->closeDevice();
				return true;
			}
		}
		return false;
	}
	IrrlichtDevice* device;
	ICursorControl* cursor;
};

enum
{
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

int main(int argc, char* argv[])
{
	// Check fullscreen
	for (int i=1;i<argc;i++) fullscreen |= !strcmp("-f", argv[i]);
	//fullscreen = true;
		
	MyEventReceiver receiver;
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 32, fullscreen, false, vsync, &receiver);
	if (!device) {
		return 1;
	}
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	receiver.device = device;
	receiver.cursor = device->getCursorControl();

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);

	CSceneHelper sceneHelper(device);

	CHMDDisplay display;
	display.setUp(device);

	//Oculus Rift Head Tracking
	CHeadTracking headTracking;
	headTracking.startUp();

	// Create world
	sceneHelper.initSky();

	scene::ITerrainSceneNode* terrain = sceneHelper.initTerrain();

	// Create Camera
	ICameraSceneNode* camNode = initCamera(device, smgr, terrain);
	auto camAnim = *camNode->getAnimators().begin();
	CSceneNodeAnimatorCameraHMD *hmdCam = static_cast<CSceneNodeAnimatorCameraHMD*>(camAnim);

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
    scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    bill->setMaterialTexture(0, driver->getTexture("ext/irrlicht/media/particle.bmp"));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialFlag(video::EMF_ZBUFFER, false);
    bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
    bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it
	
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

	// Remember which scene node is highlighted
    scene::ISceneNode* highlightedSceneNode = 0;
    scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

	// Render loop
	while(device->run()) {
		// Read-Write Head Tracking Sensor Value to Camera
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
		if(headTracking.isConnected()) {
			headTracking.getValue(&yaw, &pitch, &roll);			
		}
		hmdCam->setHeadTrackingValue(yaw, pitch, roll);

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

		int fps = driver->getFPS();
		if (lastFPS != fps) {
			core::stringw str = L"Collision detection example - Irrlicht Engine [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}
	device->drop();	
	delete(hmdCam);
	
	//Deinitialize Oculus LibOVR
	headTracking.shutDown();
	
	return 0;
}
