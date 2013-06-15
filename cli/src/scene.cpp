// Ŭnicode please 
#include "stdafx.h"
#include "scene.h"

Scene::Scene(irr::IrrlichtDevice *dev)
	: Device(dev)
{
}
Scene::~Scene()
{
}

void Scene::setUp()
{
	auto guienv = Device->getGUIEnvironment();
	auto smgr = Device->getSceneManager();
	smgr->clear();
	guienv->clear();
}

void Scene::shutDown()
{
	auto guienv = Device->getGUIEnvironment();
	auto smgr = Device->getSceneManager();

	smgr->clear();
	guienv->clear();
}
void Scene::update(int ms)
{
}

