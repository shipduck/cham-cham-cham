// Ŭnicode please 
#pragma once

#include "scene.h"

class GameScene : public Scene {
public:
	GameScene(irr::IrrlichtDevice *dev);
	virtual ~GameScene();

	virtual void setUp();
	virtual void shutDown();

	virtual void update(int ms);
	virtual void draw();

private:
};