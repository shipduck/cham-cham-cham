// Ŭnicode please 
#pragma once

class HMDStereoRender;

class Scene {
public:
	Scene(irr::IrrlichtDevice *dev);
	virtual ~Scene();

	virtual void setUp();
	virtual void shutDown();

	virtual void update(int ms) = 0;
	virtual void draw();

protected:
	irr::IrrlichtDevice *Device;
	std::unique_ptr<HMDStereoRender> Renderer;

private:
	void drawAllNormal(irr::scene::ISceneManager *smgr);
	void drawAllStereo(irr::scene::ISceneManager *smgr);
};
