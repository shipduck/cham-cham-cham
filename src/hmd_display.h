// Ŭnicode please 
#pragma once

namespace irr 
{;
class IrrlichtDevice;
namespace scene
{;
class ISceneManager;
}
}

class HMDStereoRender;

class CHMDDisplay {
public:
	CHMDDisplay();
	~CHMDDisplay();

	void setUp(irr::IrrlichtDevice *device);
	void shutDown();

	void NormalDrawAll(irr::scene::ISceneManager *smgr);
	void StereoDrawAll(irr::scene::ISceneManager *smgr);
private:

	std::unique_ptr<HMDStereoRender> Renderer;
};