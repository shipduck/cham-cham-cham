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


DebugDrawTestScene::DebugDrawTestScene(irr::IrrlichtDevice *dev)
	: Scene(dev)
{
}
DebugDrawTestScene::~DebugDrawTestScene()
{
}

void DebugDrawTestScene::setUp()
{
	Scene::setUp();
}

void DebugDrawTestScene::shutDown()
{
	Scene::shutDown();
}
void DebugDrawTestScene::update(int ms)
{
}

