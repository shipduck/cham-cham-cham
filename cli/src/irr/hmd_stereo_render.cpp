﻿// Ŭnicode please 
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
#include "stdafx.h"
#include "hmd_stereo_render.h"
#include "cvars/CVar.h"
#include "util/console_func.h"
#include "irr/head_tracker.h"
#include "base/lib.h"
#include "util/cvar_key.h"

#include <iostream>
#include <cassert>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

static const char* vertexShader =
	"void main() {"
	"  gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);"
	"  gl_TexCoord[0].st = gl_MultiTexCoord0.st;"
	"}";

static const char *fragShader =
	"uniform vec2 scale;"
	"uniform vec2 scaleIn;"
	"uniform vec2 lensCenter;"
	"uniform vec4 hmdWarpParam;"
	"uniform sampler2D texid;"
	"void main()"
	"{"
	"  vec2 uv = (gl_TexCoord[0].st*2.0)-1.0;" // range from [0,1] to [-1,1]
	"  vec2 theta = (uv-lensCenter)*scaleIn;"
	"  float rSq = theta.x*theta.x + theta.y*theta.y;"
	"  vec2 rvector = theta*(hmdWarpParam.x + hmdWarpParam.y*rSq + hmdWarpParam.z*rSq*rSq + hmdWarpParam.w*rSq*rSq*rSq);"
	"  vec2 tc = (lensCenter + scale * rvector);"
	"  tc = (tc+1.0)/2.0;" // range from [-1,1] to [0,1]
	"  if (any(bvec2(clamp(tc, vec2(0.0,0.0), vec2(1.0,1.0))-tc)))"
	"    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);"
	"  else"
	"    gl_FragColor = texture2D(texid, tc);"
	"}";

// Parameters from the Oculus Rift DK1
HMDDescriptorBind::HMDDescriptorBind()
	: hResolution(CVarUtils::GetCVarRef<int>(CVAR_HMD_H_RESOLUTION)),
	vResolution(CVarUtils::GetCVarRef<int>(CVAR_HMD_V_RESOLUTION)),
	hScreenSize(CVarUtils::GetCVarRef<float>(CVAR_HMD_H_SCREEN_SIZE)),
	vScreenSize(CVarUtils::GetCVarRef<float>(CVAR_HMD_V_SCREEN_SIZE)),
	interpupillaryDistance(CVarUtils::GetCVarRef<float>(CVAR_HMD_INTERPUPILLARY_DISTANCE)),
	lensSeparationDistance(CVarUtils::GetCVarRef<float>(CVAR_HMD_LENS_SEPARATION_DISTANCE)),
	eyeToScreenDistance(CVarUtils::GetCVarRef<float>(CVAR_HMD_EYE_TO_SCREEN_DISTANCE)),
	distortionK_1(CVarUtils::GetCVarRef<float>(CVAR_HMD_DISTORTION_K_1)),
	distortionK_2(CVarUtils::GetCVarRef<float>(CVAR_HMD_DISTORTION_K_2)),
	distortionK_3(CVarUtils::GetCVarRef<float>(CVAR_HMD_DISTORTION_K_3)),
	distortionK_4(CVarUtils::GetCVarRef<float>(CVAR_HMD_DISTORTION_K_4))
{
}

HMDDescriptor HMDDescriptorBind::convert() const
{
	HMDDescriptor retval;	
	retval.m_iResolutionH = this->hResolution;
	retval.m_iResolutionV = this->vResolution;
	retval.m_fSizeH = this->hScreenSize;
	retval.m_fSizeV = this->vScreenSize;
	retval.m_fInterpupillaryDistance = this->interpupillaryDistance;
	retval.m_fLensSeparationDistance = this->lensSeparationDistance;
	retval.m_fEyeToScreenDistance = this->eyeToScreenDistance;
	retval.m_fDistortionK[0] = this->distortionK_1;
	retval.m_fDistortionK[1] = this->distortionK_2;
	retval.m_fDistortionK[2] = this->distortionK_3;
	retval.m_fDistortionK[3] = this->distortionK_4;
	return retval;
}

const HMDDescriptor &getInvalidDescriptor()
{
	static bool init = false;
	static HMDDescriptor ctx;
	if(init == false) {
		init = true;
		ctx.m_iResolutionH = 0;
	}
	return ctx;
}

bool HMDDescriptor::operator==(const HMDDescriptor &o) const
{
	if(m_iResolutionH != o.m_iResolutionH) {
		return false;
	}
	if(m_iResolutionV != o.m_iResolutionV) {
		return false;
	}
	if(m_fSizeH != o.m_fSizeH) {
		return false;
	}
	if(m_fSizeV != o.m_fSizeV) {
		return false;
	}
	if(m_fInterpupillaryDistance != o.m_fInterpupillaryDistance) {
		return false;
	}
	if(m_fLensSeparationDistance != o.m_fLensSeparationDistance) {
		return false;
	}
	if(m_fEyeToScreenDistance != o.m_fEyeToScreenDistance) {
		return false;
	}
	for(int i = 0 ; i < 4 ; ++i) {
		if(m_fDistortionK[i] != o.m_fDistortionK[i]) {
			return false;
		}
	}
	return true;
	//return (memcmp(this, &o, sizeof(this)) == 0);
}

//OculusDistorsionCallback as singleton
OculusDistorsionCallback g_distortionCB;
E_MATERIAL_TYPE getOculusDistorsionCallbackMaterial(irr::video::IVideoDriver *driver)
{
	static int material = -1;
	if(material == -1) {
		IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices(); 
		material = (E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(vertexShader, "main", EVST_VS_3_0, fragShader, "main", EPST_PS_3_0, &g_distortionCB);
		//printf("HMDStereoRender Material Type : %d\n", material);
	}
	return (E_MATERIAL_TYPE)material;
}

HMDStereoRender::HMDStereoRender(irr::IrrlichtDevice *device, const HMDDescriptor &HMD, f32 worldScale)
	: m_fWorldScale(worldScale),
	m_cHMD(getInvalidDescriptor()),
	m_pDriver(device->getVideoDriver()),
	m_pRenderTexture(nullptr),
	m_pHeadX(nullptr),
	m_pHeadY(nullptr),
	m_pHeadZ(nullptr),
	m_pYaw(nullptr),
	m_pLeftEye(nullptr),
	m_pRghtEye(nullptr),
	m_pTimer(device->getTimer()),
	m_cDistortionCB(g_distortionCB)
{
	auto smgr = device->getSceneManager();
	// Create perspectiva camera used for rendering
	m_pCamera = smgr->addCameraSceneNode();

	// Init shader parameters
	m_cDistortionCB.m_fScale     [0] = 1.0f; m_cDistortionCB.m_fScale     [1] = 1.0f;
	m_cDistortionCB.m_fScaleIn   [0] = 1.0f; m_cDistortionCB.m_fScaleIn   [1] = 1.0f;
	m_cDistortionCB.m_fLensCenter[0] = 0.0f; m_cDistortionCB.m_fLensCenter[1] = 0.0f;

	m_cDistortionCB.m_fHmdWarpParam[0] = 1.0f;
	m_cDistortionCB.m_fHmdWarpParam[1] = 0.0f;
	m_cDistortionCB.m_fHmdWarpParam[2] = 0.0f;
	m_cDistortionCB.m_fHmdWarpParam[3] = 0.0f;

	// Plane
	m_cPlaneVertices[0] = video::S3DVertex(-1.0f, -1.0f, 1.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f);
	m_cPlaneVertices[1] = video::S3DVertex(-1.0f,  1.0f, 1.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
	m_cPlaneVertices[2] = video::S3DVertex( 1.0f,  1.0f, 1.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f);
	m_cPlaneVertices[3] = video::S3DVertex( 1.0f, -1.0f, 1.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);

	m_iPlaneIndices[0] = 0; 
	m_iPlaneIndices[1] = 1; 
	m_iPlaneIndices[2] = 2; 
	m_iPlaneIndices[3] = 0; 
	m_iPlaneIndices[4] = 2; 
	m_iPlaneIndices[5] = 3;

	// Create shader material
	m_cRenderMaterial.Wireframe = false;
	m_cRenderMaterial.Lighting = false;
	m_cRenderMaterial.TextureLayer[0].TextureWrapU = ETC_CLAMP;
	m_cRenderMaterial.TextureLayer[0].TextureWrapV = ETC_CLAMP;

	IGPUProgrammingServices* gpu = m_pDriver->getGPUProgrammingServices();
	m_cRenderMaterial.MaterialType = getOculusDistorsionCallbackMaterial(m_pDriver);

	// Start of Oculus Rift Code provided by Christian Keimel / bulletbyte.de
	m_bRiftAvailable = Lib::headTracker->isConnected();

	m_pYaw = smgr->addEmptySceneNode(0, 0);
	m_pHeadX = smgr->addEmptySceneNode(m_pYaw , 0);
	m_pHeadY = smgr->addEmptySceneNode(m_pHeadX, 0);
	m_pHeadZ = smgr->addEmptySceneNode(m_pHeadY, 0);

	m_pLeftEye = smgr->addEmptySceneNode(m_pHeadZ, 0);
	m_pLeftEye->setPosition(irr::core::vector3df(-0.25, 0, 0));
	m_pRghtEye = smgr->addEmptySceneNode(m_pHeadZ, 0);
	m_pRghtEye->setPosition(irr::core::vector3df( 0.25, 0, 0));

	// End of Oculus Rift Code provided by Christian Keimel / bulletbyte.de

	setHMD(HMD);
}

HMDStereoRender::~HMDStereoRender()
{
}

void HMDStereoRender::setHMD(const HMDDescriptor &HMD)
{
	if(m_cHMD == HMD) {
		return;
	}
	m_cHMD = HMD;

	// Compute aspect ratio and FOV
	irr::f32 l_fAspect    = m_cHMD.m_iResolutionH / (2.0f * m_cHMD.m_iResolutionV),
		l_fR         = -1.0f - (4.0f * (m_cHMD.m_fSizeH / 4.0f - m_cHMD.m_fLensSeparationDistance / 2.0f) / m_cHMD.m_fSizeH),
		l_fDistScale = (m_cHMD.m_fDistortionK[0] + m_cHMD.m_fDistortionK[1] * pow(l_fR,2) + m_cHMD.m_fDistortionK[2] * pow(l_fR,4) + m_cHMD.m_fDistortionK[3] * pow(l_fR,6)),
		l_fFov       = 2.0f * atan2(m_cHMD.m_fSizeV * l_fDistScale, 2.0f * m_cHMD.m_fEyeToScreenDistance),
		l_fH         = 4 * (m_cHMD.m_fSizeH / 4 - m_cHMD.m_fInterpupillaryDistance/2) / m_cHMD.m_fSizeH;

	// Compute camera projection matrices
	irr::core::matrix4 l_cCenterProjection = irr::core::matrix4().buildProjectionMatrixPerspectiveFovLH (l_fFov, l_fAspect, 1, 10000);

	m_cProjectionLeft = irr::core::matrix4().setTranslation(irr::core::vector3df( l_fH, 0.0, 0.0)) * l_cCenterProjection;
	m_cProjectionRght = irr::core::matrix4().setTranslation(irr::core::vector3df(-l_fH, 0.0, 0.0)) * l_cCenterProjection;

	// Compute camera offset
	m_fEyeSeparation = m_fWorldScale * m_cHMD.m_fInterpupillaryDistance/2.0f;

	m_pLeftEye->setPosition(irr::core::vector3df( m_fEyeSeparation, 0.0f, 0.0f));
	m_pRghtEye->setPosition(irr::core::vector3df(-m_fEyeSeparation, 0.0f, 0.0f));

	// Compute Viewport
	m_cViewportLeft = irr::core::rect<irr::s32>(                        0, 0, m_cHMD.m_iResolutionH / 2, m_cHMD.m_iResolutionV);
	m_cViewportRght = irr::core::rect<irr::s32>(m_cHMD.m_iResolutionH / 2, 0, m_cHMD.m_iResolutionH    , m_cHMD.m_iResolutionV);

	// Distortion shader parameters
	m_fLensShift = 4.0f * (m_cHMD.m_fSizeH / 4.0f - m_cHMD.m_fLensSeparationDistance/2.0f) / m_cHMD.m_fSizeH;

	m_cDistortionCB.m_fScale[0] = 1.0f            / l_fDistScale;
	m_cDistortionCB.m_fScale[1] = 1.0f * l_fAspect/ l_fDistScale;

	m_cDistortionCB.m_fScaleIn[0] = 1.0f;
	m_cDistortionCB.m_fScaleIn[1] = 1.0f / l_fAspect;

	m_cDistortionCB.m_fHmdWarpParam[0] = m_cHMD.m_fDistortionK[0];
	m_cDistortionCB.m_fHmdWarpParam[1] = m_cHMD.m_fDistortionK[1];
	m_cDistortionCB.m_fHmdWarpParam[2] = m_cHMD.m_fDistortionK[2];
	m_cDistortionCB.m_fHmdWarpParam[3] = m_cHMD.m_fDistortionK[3];

	// Create render target
	if (m_pDriver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET)) {
		if (m_pRenderTexture != NULL) {
			m_pDriver->removeTexture(m_pRenderTexture);
			m_pRenderTexture = nullptr;
		}
		auto tex = m_pRenderTexture = m_pDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>((irr::u32)(m_cHMD.m_iResolutionH * l_fDistScale / 2.0f), (irr::u32)(m_cHMD.m_iResolutionV * l_fDistScale)));
		SR_ASSERT(tex != nullptr);
		m_cRenderMaterial.setTexture(0, m_pRenderTexture);
	} else {
		// Render to target not supported
		assert(0);
	}
}

void HMDStereoRender::update(irr::scene::ISceneManager *smgr)
{
	irr::scene::ICameraSceneNode *l_pCamera = smgr->getActiveCamera();
	l_pCamera->OnAnimate(m_pTimer->getTime());

	// Start of Oculus Rift Code provided by Christian Keimel / bulletbyte.de

	irr::core::matrix4 l_cMat;
	irr::core::vector3df v;
	if (m_bRiftAvailable) {
		//IrrRift_poll(v.X, v.Y, v.Z);
		SHeadTrackingEvent evt = Lib::headTracker->getValue();
		v.X = evt.pitch;
		v.Y = evt.yaw;
		v.Z = evt.roll;
	} else {
		v = irr::core::vector3df(0, 0, 0);
	}

	v.X *=  irr::core::RADTODEG;
	v.Y *= -irr::core::RADTODEG;
	v.Z *= -irr::core::RADTODEG;

	m_pYaw  ->setRotation(l_pCamera->getRotation()); // irr::core::vector3df(        0, l_pCamera->getRotation().Y,   0));
	m_pHeadY->setRotation(irr::core::vector3df(        0, v.Y                       ,   0));
	m_pHeadX->setRotation(irr::core::vector3df(      v.X,   0                       ,   0));
	m_pHeadZ->setRotation(irr::core::vector3df(        0,   0                       , v.Z));

	l_cMat.setRotationDegrees(m_pHeadZ->getAbsoluteTransformation().getRotationDegrees());

	irr::core::vector3df vFore = irr::core::vector3df(0, 0, -1),
		vUp   = irr::core::vector3df(0, 1,  0);

	l_cMat.transformVect(vFore);
	l_cMat.transformVect(vUp  );

	l_pCamera->setTarget  (l_pCamera->getPosition() + vFore);
	l_pCamera->setUpVector(                           vUp  );

}
void HMDStereoRender::drawAll(ISceneManager* smgr)
{
	irr::scene::ICameraSceneNode *l_pCamera = smgr->getActiveCamera();

	// Render Left
	m_pDriver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));

	// End of Oculus Rift Code provided by Christian Keimel / bulletbyte.de

	m_pCamera->setProjectionMatrix(m_cProjectionLeft);

	irr::core::vector3df l_vR = l_pCamera->getRotation();
	irr::core::vector3df l_vTx(0, 0, 1);//-m_fEyeSeparation, 0.0, 0.0);
	l_vTx.rotateXZBy(-l_vR.Y);
	l_vTx.rotateYZBy(-l_vR.X);
	l_vTx.rotateXYBy(-l_vR.Z);

	m_pCamera->setPosition(l_pCamera->getPosition() + m_pLeftEye->getAbsolutePosition());
	m_pCamera->setTarget  (l_pCamera->getTarget  () + m_pLeftEye->getAbsolutePosition());//getTarget  () + l_vTx);
	m_pCamera->setUpVector(l_pCamera->getUpVector());

	smgr->setActiveCamera(m_pCamera);
	smgr->drawAll();

	m_pDriver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));
	m_pDriver->setViewPort(m_cViewportLeft);

	m_cDistortionCB.m_fLensCenter[0] = m_fLensShift;

	m_pDriver->setMaterial(m_cRenderMaterial);
	m_pDriver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

	// Render Right
	m_pDriver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));
	m_pCamera->setProjectionMatrix(m_cProjectionRght);

	irr::core::vector3df l_vTxt2(0, 0, 1);//m_fEyeSeparation, 0.0, 0.0);
	l_vTxt2.rotateXZBy(-l_vR.Y);
	l_vTxt2.rotateYZBy(-l_vR.X);
	l_vTxt2.rotateXYBy(-l_vR.Z);

	m_pCamera->setPosition(l_pCamera->getPosition() + m_pRghtEye->getAbsolutePosition()); //l_vTxt2);
	m_pCamera->setTarget  (l_pCamera->getTarget  () + m_pRghtEye->getAbsolutePosition());
	m_pCamera->setUpVector(l_pCamera->getUpVector());

	smgr->drawAll();

	m_pDriver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));
	m_pDriver->setViewPort(m_cViewportRght);

	m_cDistortionCB.m_fLensCenter[0] = -m_fLensShift;

	m_pDriver->setMaterial(m_cRenderMaterial);
	m_pDriver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

	smgr->setActiveCamera(l_pCamera);
}