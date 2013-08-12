// Ŭnicode please 
/*
Copyright (C) 2012 Luca Siciliano

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, 
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT 
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __HMD_STEREO_RENDER__
#define __HMD_STEREO_RENDER__

#include <irrlicht.h>

struct HMDDescriptor {
	//뻐킹 양놈의 코딩 스타일 갈아치움
	//int hResolution;
	//int vResolution;
	//float hScreenSize;
	//float vScreenSize;
	//float interpupillaryDistance;
	//float lensSeparationDistance;
	//float eyeToScreenDistance;
	//float distortionK[4];

	irr::s32 m_iResolutionH;
	irr::s32 m_iResolutionV;
	irr::f32 m_fSizeH;
	irr::f32 m_fSizeV;
	irr::f32 m_fInterpupillaryDistance;
	irr::f32 m_fLensSeparationDistance;
	irr::f32 m_fEyeToScreenDistance;
	irr::f32 m_fDistortionK[4];

	bool operator==(const HMDDescriptor &o) const;
	bool operator!=(const HMDDescriptor &o) const { return !(*this == o); }
};

struct HMDDescriptorBind {
	HMDDescriptorBind();

	int &hResolution;
	int &vResolution;
	float &hScreenSize;
	float &vScreenSize;
	float &interpupillaryDistance;
	float &lensSeparationDistance;
	float &eyeToScreenDistance;
	float &distortionK_1;
	float &distortionK_2;
	float &distortionK_3;
	float &distortionK_4;

	operator HMDDescriptor() const { return convert(); }
	HMDDescriptor convert() const;
};

class OculusDistorsionCallback: public irr::video::IShaderConstantSetCallBack {
public:
	irr::f32 m_fScale[2];
	irr::f32 m_fScaleIn[2];
	irr::f32 m_fLensCenter[2];
	irr::f32 m_fHmdWarpParam[4];
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) 
	{ 
		irr::video::IVideoDriver* driver = services->getVideoDriver();
		services->setPixelShaderConstant("scale", m_fScale, 2);
		services->setPixelShaderConstant("scaleIn", m_fScaleIn ,2);
		services->setPixelShaderConstant("lensCenter", m_fLensCenter ,2);
		services->setPixelShaderConstant("hmdWarpParam", m_fHmdWarpParam ,4);
	}
};
/*
irrlicht doesn't support remove material renderer.
NEED only ONE OculusDistorsionCallback!
*/
extern OculusDistorsionCallback g_distortionCB;

class HMDStereoRender {
public:
	HMDStereoRender(irr::IrrlichtDevice *device, const HMDDescriptor &HMD, irr::f32 worldScale = 1.0);
	~HMDStereoRender();

	HMDDescriptor getHMD(); 
	void setHMD(const HMDDescriptor &HMD);

	irr::f32 getWorldScale(); 
	void setWorldScale(irr::f32 worldScale);

	void drawAll(irr::scene::ISceneManager* smgr);

	void update(irr::scene::ISceneManager *smgr);

private: 
	irr::video::IVideoDriver *m_pDriver;
	irr::video::ITexture *m_pRenderTexture;

	irr::scene::ISceneNode *m_pHeadX;
	irr::scene::ISceneNode *m_pHeadY;
	irr::scene::ISceneNode *m_pHeadZ;
	irr::scene::ISceneNode *m_pYaw;
	irr::scene::ISceneNode *m_pLeftEye;
	irr::scene::ISceneNode *m_pRghtEye;

	HMDDescriptor m_cHMD;

	irr::f32 m_fWorldScale;
	irr::f32 m_fEyeSeparation;
	irr::f32 m_fLensShift;

	bool m_bRiftAvailable;

	irr::core::matrix4 m_cProjectionLeft;
	irr::core::matrix4 m_cProjectionRght;

	irr::core::rect<irr::s32> m_cViewportLeft;
	irr::core::rect<irr::s32> m_cViewportRght;

	irr::scene::ICameraSceneNode *m_pCamera;

	irr::video::SMaterial m_cRenderMaterial;
	irr::video::S3DVertex m_cPlaneVertices[4];
	irr::u16 m_iPlaneIndices[6];
	irr::ITimer* m_pTimer;

	OculusDistorsionCallback &m_cDistortionCB;
};
#endif
