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
	: hResolution(CVarUtils::CreateCVar<int>("hmd.hResolution", 1280)),
	vResolution(CVarUtils::CreateCVar<int>("hmd.vResolution", 800)),
	hScreenSize(CVarUtils::CreateCVar<float>("hmd.hScreenSize", 0.14976f)),
	vScreenSize(CVarUtils::CreateCVar<float>("hmd.vScreenSize", 0.0936f)),
	interpupillaryDistance(CVarUtils::CreateCVar<float>("hmd.interpupillaryDistance", 0.064f)),
	lensSeparationDistance(CVarUtils::CreateCVar<float>("hmd.lensSeparationDistance", 0.064f)),
	eyeToScreenDistance(CVarUtils::CreateCVar<float>("hmd.eyeToScreenDistance", 0.041f)),
	distortionK_1(CVarUtils::CreateCVar<float>("hmd.distortionK_1", 1.0f)),
	distortionK_2(CVarUtils::CreateCVar<float>("hmd.distortionK_2", 0.22f)),
	distortionK_3(CVarUtils::CreateCVar<float>("hmd.distortionK_3", 0.24f)),
	distortionK_4(CVarUtils::CreateCVar<float>("hmd.distortionK_4", 0.0f))
{
}

HMDDescriptor HMDDescriptorBind::convert() const
{
	HMDDescriptor retval;	
	retval.hResolution = this->hResolution;
	retval.vResolution = this->vResolution;
	retval.hScreenSize = this->hScreenSize;
	retval.vScreenSize = this->vScreenSize;
	retval.interpupillaryDistance = this->interpupillaryDistance;
	retval.lensSeparationDistance = this->lensSeparationDistance;
	retval.eyeToScreenDistance = this->eyeToScreenDistance;
	retval.distortionK[0] = this->distortionK_1;
	retval.distortionK[1] = this->distortionK_2;
	retval.distortionK[2] = this->distortionK_3;
	retval.distortionK[3] = this->distortionK_4;
	return retval;
}

const HMDDescriptor &getInvalidDescriptor()
{
	static bool init = false;
	static HMDDescriptor ctx;
	if(init == false) {
		init = true;
		ctx.hResolution = 0;
	}
	return ctx;
}

bool HMDDescriptor::operator==(const HMDDescriptor &o) const
{
	return (memcmp(this, &o, sizeof(this)) == 0);
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
	: _worldScale(worldScale),
	_renderTexture(nullptr),
	_hmd(getInvalidDescriptor())
{
	_smgr = device->getSceneManager();
	_driver = device->getVideoDriver();
	_timer = device->getTimer();

	// Create perspectiva camera used for rendering
	_pCamera = _smgr->addCameraSceneNode();

	// Init shader parameters
	g_distortionCB.scale[0] = 1.0f; g_distortionCB.scale[1] = 1.0f;
	g_distortionCB.scaleIn[0] = 1.0f; g_distortionCB.scaleIn[1] = 1.0f;
	g_distortionCB.lensCenter[0] = 0.0f;g_distortionCB.lensCenter[1] = 0.0f;
	g_distortionCB.hmdWarpParam[0] = 1.0f;g_distortionCB.hmdWarpParam[1] = 0.0f;g_distortionCB.hmdWarpParam[2] = 0.0f;g_distortionCB.hmdWarpParam[3] = 0.0f;

	// Plane
	_planeVertices[0] = video::S3DVertex(-1.0f, -1.0f, 1.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f);
	_planeVertices[1] = video::S3DVertex(-1.0f,  1.0f, 1.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
	_planeVertices[2] = video::S3DVertex( 1.0f,  1.0f, 1.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f);
	_planeVertices[3] = video::S3DVertex( 1.0f, -1.0f, 1.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
	_planeIndices[0] = 0; _planeIndices[1] = 1; _planeIndices[2] = 2; _planeIndices[3] = 0; _planeIndices[4] = 2; _planeIndices[5] = 3;

	// Create shader material
	_renderMaterial.Wireframe = false;
	_renderMaterial.Lighting = false;
	_renderMaterial.TextureLayer[0].TextureWrapU = ETC_CLAMP;
	_renderMaterial.TextureLayer[0].TextureWrapV = ETC_CLAMP;

	IGPUProgrammingServices* gpu = _driver->getGPUProgrammingServices();
	_renderMaterial.MaterialType = getOculusDistorsionCallbackMaterial(_driver);
	setHMD(HMD);
}

HMDStereoRender::~HMDStereoRender() {
}

void HMDStereoRender::setHMD(const HMDDescriptor &HMD) {
	if(_hmd == HMD) {
		return;
	}
	_hmd = HMD;

	// Compute aspect ratio and FOV
	float aspect = HMD.hResolution / (2.0f*HMD.vResolution);

	// Fov is normally computed with:
	//   2*atan2(HMD.vScreenSize,2*HMD.eyeToScreenDistance)
	// But with lens distortion it is increased (see Oculus SDK Documentation)
	float r = -1.0f - (4.0f * (HMD.hScreenSize/4.0f - HMD.lensSeparationDistance/2.0f) / HMD.hScreenSize);
	float distScale = (HMD.distortionK[0] + HMD.distortionK[1] * pow(r,2) + HMD.distortionK[2] * pow(r,4) + HMD.distortionK[3] * pow(r,6));
	float fov = 2.0f*atan2(HMD.vScreenSize*distScale, 2.0f*HMD.eyeToScreenDistance);

	// Compute camera projection matrices
	matrix4 centerProjection = matrix4().buildProjectionMatrixPerspectiveFovLH (fov, aspect, 1, 10000);
	float h = 4 * (HMD.hScreenSize/4 - HMD.interpupillaryDistance/2) / HMD.hScreenSize;
	_projectionLeft = matrix4().setTranslation(vector3df(h, 0.0, 0.0)) * centerProjection;
	_projectionRight = matrix4().setTranslation(vector3df(-h, 0.0, 0.0)) * centerProjection;

	// Compute camera offset
	_eyeSeparation = _worldScale * HMD.interpupillaryDistance/2.0f;

	// Compute Viewport
	_viewportLeft = rect<s32>(0, 0, HMD.hResolution/2, HMD.vResolution);
	_viewportRight = rect<s32>(HMD.hResolution/2, 0, HMD.hResolution, HMD.vResolution);

	// Distortion shader parameters
	_lensShift = 4.0f * (HMD.hScreenSize/4.0f - HMD.lensSeparationDistance/2.0f) / HMD.hScreenSize;

	g_distortionCB.scale[0] = 1.0f/distScale;
	g_distortionCB.scale[1] = 1.0f*aspect/distScale;

	g_distortionCB.scaleIn[0] = 1.0f;
	g_distortionCB.scaleIn[1] = 1.0f/aspect;

	g_distortionCB.hmdWarpParam[0] = HMD.distortionK[0];
	g_distortionCB.hmdWarpParam[1] = HMD.distortionK[1];
	g_distortionCB.hmdWarpParam[2] = HMD.distortionK[2];
	g_distortionCB.hmdWarpParam[3] = HMD.distortionK[3];

	// Create render target
	if (_driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
	{
		if (_renderTexture != nullptr) {
			_driver->removeTexture(_renderTexture);
			_renderTexture = nullptr;
		}
		_renderTexture = _driver->addRenderTargetTexture(dimension2d<u32>(HMD.hResolution*distScale/2.0f, HMD.vResolution*distScale));
		_renderMaterial.setTexture(0, _renderTexture);
	}
	else {
		// Render to target not supported
		assert(0);
	}
}


void HMDStereoRender::drawAll(ISceneManager* smgr) {

	ICameraSceneNode* camera = smgr->getActiveCamera();
	camera->OnAnimate(_timer->getTime());

	// Render Left
	_driver->setRenderTarget(_renderTexture, true, true, video::SColor(0,0,0,0));

	_pCamera->setProjectionMatrix(_projectionLeft);

	vector3df r = camera->getRotation();
	vector3df tx(-_eyeSeparation, 0.0,0.0);
	tx.rotateXZBy(-r.Y);
	tx.rotateYZBy(-r.X);
	tx.rotateXYBy(-r.Z);

	_pCamera->setPosition(camera->getPosition() + tx);
	_pCamera->setTarget(camera->getTarget() + tx);  

	smgr->setActiveCamera(_pCamera);
	smgr->drawAll();

	_driver->setRenderTarget(0, false, false, video::SColor(0,100,100,100));
	_driver->setViewPort(_viewportLeft);

	g_distortionCB.lensCenter[0] = _lensShift;

	_driver->setMaterial(_renderMaterial); 
	_driver->drawIndexedTriangleList(_planeVertices, 4, _planeIndices, 2);

	// Render Right
	_driver->setRenderTarget(_renderTexture, true, true, video::SColor(0,0,0,0));
	_pCamera->setProjectionMatrix(_projectionRight);

	vector3df r2 = camera->getRotation();
	vector3df tx2(-_eyeSeparation, 0.0,0.0);
	tx.rotateXZBy(-r2.Y);
	tx.rotateYZBy(-r2.X);
	tx.rotateXYBy(-r2.Z);

	_pCamera->setPosition(camera->getPosition() + tx2);
	_pCamera->setTarget(camera->getTarget() + tx2);  

	smgr->drawAll();

	_driver->setRenderTarget(0, false, false, video::SColor(0,100,100,100));  
	_driver->setViewPort(_viewportRight);

	g_distortionCB.lensCenter[0] = -_lensShift;

	_driver->setMaterial(_renderMaterial); 
	_driver->drawIndexedTriangleList(_planeVertices, 4, _planeIndices, 2);

	smgr->setActiveCamera(camera);
}