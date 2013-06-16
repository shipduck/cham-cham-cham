// Ŭnicode please 
#pragma once

class Scene {
public:
	Scene(irr::IrrlichtDevice *dev);
	virtual ~Scene();

	virtual void update(int ms);
	virtual void setUp();
	virtual void shutDown();

protected:
	irr::IrrlichtDevice *Device;
};
