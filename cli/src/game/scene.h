// Ŭnicode please 
#pragma once

class Scene {
public:
	Scene(irr::IrrlichtDevice *dev);
	virtual ~Scene();

	virtual void setUp();
	virtual void shutDown();

	virtual void update(int ms) = 0;
	virtual void draw() = 0;

protected:
	irr::IrrlichtDevice *Device;
};
