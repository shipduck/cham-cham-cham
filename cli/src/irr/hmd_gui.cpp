// Ŭnicode please 
#include "stdafx.h"
#include "hmd_gui.h"
#include "base/lib.h"

using namespace irr;
using namespace std;

namespace hmd_ui {;
Manager::Manager()
{
}

Manager::~Manager()
{
	shutDown();
}
Component *Manager::add(Component *comp)
{
	compList_.push_back(comp);
	return comp;
}
void Manager::drawAll()
{
	for(auto x : compList_) {
		x->bind();
		x->draw();
	}
	Component::unbind();
}

void Manager::shutDown()
{
	for(auto x : compList_) {
		delete(x);
	}
	compList_.clear();
}

Component::Component(int w, int h, const std::string &name)
	: smgr_(Lib::smgr->createNewSceneManager(false)),
	rtSize_(w, h),
	rt_(Lib::driver->addRenderTargetTexture(core::dimension2d<u32>(w, h), name.c_str())),
	bgColor_(0, 0, 0, 255)
{
	SR_ASSERT(w > 0);
	SR_ASSERT(h > 0);
}
Component::~Component()
{
	if(smgr_ != nullptr) {
		smgr_->drop();
		smgr_ = nullptr;
	}

	Lib::driver->removeTexture(rt_);
	rt_ = nullptr;
}

void Component::draw()
{
	smgr_->drawAll();
}
void Component::bind()
{
	// set render target texture
	Lib::driver->setRenderTarget(rt_, true, true, bgColor_);
}
void Component::unbind()
{
	Lib::driver->setRenderTarget(0, true, true, 0);
}


TestComp::TestComp()
	: Component(512, 512, "RTT1")
{
	this->setBackgroundColor(video::SColor(0, 0, 128, 128));

	const char *md2file = "ext/irrlicht/media/faerie.md2";
	const char *texfile = "ext/irrlicht/media/faerie2.bmp";
	scene::IAnimatedMeshSceneNode* fairy = getSceneMgr()->addAnimatedMeshSceneNode(
		Lib::smgr->getMesh(md2file));
	
	SR_ASSERT(fairy != nullptr);
	fairy->setMaterialTexture(0, Lib::driver->getTexture(texfile)); // set diffuse texture
	fairy->setMaterialFlag(video::EMF_LIGHTING, true); // enable dynamic lighting
	fairy->getMaterial(0).Shininess = 20.0f; // set size of specular highlights
	fairy->setPosition(core::vector3df(-10,0,-100));
	fairy->setMD2Animation(scene::EMAT_STAND);

	// add white light
	getSceneMgr()->addLightSceneNode(0, core::vector3df(-15,5,-105), video::SColorf(1.0f, 1.0f, 1.0f));

	// set ambient light
	getSceneMgr()->setAmbientLight(video::SColor(0,60,60,60));

	// add fixed camera
	getSceneMgr()->addCameraSceneNode(0, core::vector3df(10,10,-80), core::vector3df(-10,10,-100));
}

TestComp::~TestComp()
{
}


CylinderMappingNode::CylinderMappingNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, irr::s32 id)
	: ISceneNode(parent, smgr, id),
	radius(10.0f),
	scale(1.0f)
{
	box_.reset(core::vector3df(0, 0, 0));

	material_.Wireframe = false;
	material_.Lighting = false;
	//디버깅 삽질 줄일라고 일단 해제
	material_.BackfaceCulling = false;

	//원점에 해당하는곳에 적절히 뭐 하나 찍기. 이거조차 없으면 디버깅하다 망할듯
	auto cube = smgr->addCubeSceneNode(1, this);
	cube->setIsDebugObject(true);
	cube->setMaterialFlag(video::EMF_LIGHTING, false);
}

void CylinderMappingNode::setTexture(irr::video::ITexture *tex)
{
	material_.setTexture(0, tex);

	vertexList_.clear();
	indexList_.clear();

	float width = tex->getSize().Width * scale;
	float height = tex->getSize().Height * scale;
	float hw = width / 2.0f;
	float halfRad = hw / radius;

	SR_ASSERT(halfRad <= 2 * core::PI && "too large texture or too small radius");

	// 각도를 얼마나 잘게 쪼개서 보여줄지는 대충 정하면 될듯
	// 대략 10도=1조각 정도로 생각하자
	float segmentRad = core::degToRad(10.0f);
	vector<float> radList;
	float currRad = -halfRad;
	while(currRad < 0) {
		radList.push_back(currRad);
		currRad += segmentRad;
	}
	radList.push_back(0);
	currRad = segmentRad;
	while(currRad < halfRad) {
		radList.push_back(currRad);
		currRad += segmentRad;
	}
	radList.push_back(halfRad);
	SR_ASSERT(radList.size() >= 3);

	video::SColor white(255, 255, 255, 255);
	for(size_t i = 0 ; i < radList.size() ; ++i) {
		float rad = radList[i];
		float sinVal = sin(rad);
		float cosVal = cos(rad);
		float x = sinVal * radius;
		float z = cosVal * radius;
		
		float u = static_cast<float>(i) / radList.size();

		auto v1 = video::S3DVertex(x,-height/2.0f,z, sinVal,0,cosVal, white, u, 1);
		auto v2 = video::S3DVertex(x,+height/2.0f,z, sinVal,0,cosVal, white, u, 0);

		vertexList_.push_back(v1);
		vertexList_.push_back(v2);
	}
	for(int i = 0 ; i < radList.size() * 2 ; ++i) {
		indexList_.push_back(i);
	}
}
void CylinderMappingNode::OnRegisterSceneNode()
{
	if(IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}
	ISceneNode::OnRegisterSceneNode();
}
void CylinderMappingNode::render()
{
	if(indexList_.empty()) {
		return;
	}

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(material_);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);	

	driver->drawVertexPrimitiveList(
		vertexList_.data(), 
		vertexList_.size(), 
		indexList_.data(), 
		indexList_.size() - 2, 
		video::EVT_STANDARD, scene::EPT_TRIANGLE_STRIP, video::EIT_16BIT);
}

}	// namespace hmd_ui