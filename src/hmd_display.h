// Ŭnicode please 
#pragma once

class HMDStereoRender;

class HMDDisplay {
public:
	HMDDisplay();
	~HMDDisplay();

	void setUp(irr::IrrlichtDevice *device);
	void shutDown();

	void NormalDrawAll(irr::scene::ISceneManager *smgr);
	void StereoDrawAll(irr::scene::ISceneManager *smgr);
private:

	std::unique_ptr<HMDStereoRender> Renderer;
};