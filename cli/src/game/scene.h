// Ŭnicode please 
#pragma once

class HMDStereoRender;

class Scene {
public:
	Scene(irr::IrrlichtDevice *dev);
	virtual ~Scene();

	virtual void update(int ms) = 0;
	virtual void draw();

public:
	irr::scene::ISceneNode *getRootScene() { return rootScene_; }

protected:
	irr::IrrlichtDevice *device_;
	irr::scene::ISceneNode *rootScene_;

private:
	void drawAllNormal(irr::scene::ISceneManager *smgr);
	void drawAllStereo(irr::scene::ISceneManager *smgr);

private:
	std::unique_ptr<HMDStereoRender> renderer_;
};
