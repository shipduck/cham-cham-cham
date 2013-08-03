// Ŭnicode please 
#pragma once

class HMDStereoRender;

class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual void update(int ms) = 0;
	virtual void draw();

private:
	void drawAllNormal(irr::scene::ISceneManager *smgr);
	void drawAllStereo(irr::scene::ISceneManager *smgr);

private:
	std::unique_ptr<HMDStereoRender> renderer_;
};
