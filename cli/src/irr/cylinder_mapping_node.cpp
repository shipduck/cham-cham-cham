// Ŭnicode please 
#include "stdafx.h"
#include "cylinder_mapping_node.h"
#include "base/lib.h"

using namespace irr;
using namespace std;

CylinderMappingNode::CylinderMappingNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, irr::s32 id)
	: ISceneNode(parent, smgr, id),
	radius(10.0f)
{
	setAutomaticCulling(false);
	box_.reset(core::vector3df(0, 0, 0));

	material_.Wireframe = false;
	//material_.Wireframe = true;
	material_.Lighting = false;
	material_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	//디버깅 삽질 줄일라고 일단 해제
	//material_.BackfaceCulling = false;

	//원점에 해당하는곳에 적절히 뭐 하나 찍기. 이거조차 없으면 디버깅하다 망할듯
	auto cube = smgr->addCubeSceneNode(1, this);
	cube->setIsDebugObject(true);
	cube->setMaterialFlag(video::EMF_LIGHTING, false);
}
CylinderMappingNode::~CylinderMappingNode()
{
}

void CylinderMappingNode::OnRegisterSceneNode()
{
	if(IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}
	ISceneNode::OnRegisterSceneNode();
}
void CylinderMappingNode::renderBasic(const vertex_list_type &vertexList,
		const index_list_type &indexList)
{
	if(indexList.empty()) {
		return;
	}

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(material_);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);	

	driver->drawVertexPrimitiveList(
		vertexList.data(), 
		vertexList.size(), 
		indexList.data(), 
		indexList.size() - 2, 
		video::EVT_STANDARD, scene::EPT_TRIANGLE_STRIP, video::EIT_16BIT);
}
void CylinderMappingNode::buildVertexList(irr::video::ITexture *tex, 
		float scale, 
		vertex_list_type *vertexList, 
		index_list_type *indexList)
{
	vertexList->clear();
	indexList->clear();

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

		vertexList->push_back(v1);
		vertexList->push_back(v2);
	}
	for(size_t i = 0 ; i < radList.size() * 2 ; ++i) {
		indexList->push_back(i);
	}
}

CylinderTextureNode::CylinderTextureNode(irr::scene::ISceneNode *parent, 
		irr::scene::ISceneManager *smgr, 
		irr::s32 id,
		irr::video::ITexture *tex)
	: CylinderMappingNode(parent, smgr, id),
	tex_(tex)
{
	tex_->grab();

	material_.setTexture(0, tex);

	float scale = 0.02f;
	buildVertexList(tex, scale, &vertexList_, &indexList_);
}

void CylinderTextureNode::rebuild()
{
	float scale = 0.02f;
	buildVertexList(tex_, scale, &vertexList_, &indexList_);
}
CylinderTextureNode::~CylinderTextureNode()
{
	tex_->drop();
	tex_ = nullptr;
}

void CylinderTextureNode::render()
{
	renderBasic(vertexList_, indexList_);
}

CylinderButtonNode::CylinderButtonNode(irr::scene::ISceneNode *parent, 
									   irr::scene::ISceneManager *smgr, 
									   irr::s32 id,
									   const char *normalFile,
									   const char *selectFile)
	: CylinderMappingNode(parent, smgr, id),
	normalTex_(nullptr),
	selectTex_(nullptr),
	selected(false)
{
	normalTex_ = Lib::driver->getTexture(normalFile);

	if(selectFile != nullptr) {
		selectTex_ = Lib::driver->getTexture(selectFile);
	} else {
		selectTex_ = normalTex_;
	}

	rebuild();
}

CylinderButtonNode::~CylinderButtonNode()
{
	if(selectTex_ != normalTex_) {
		Lib::driver->removeTexture(normalTex_);
		Lib::driver->removeTexture(selectTex_);
		normalTex_ = nullptr;
		selectTex_ = nullptr;
	} else {
		Lib::driver->removeTexture(normalTex_);
		normalTex_ = nullptr;
	}
}

void CylinderButtonNode::render()
{
	renderBasic(vertexList_, indexList_);
}

void CylinderButtonNode::rebuild()
{
	auto tex = getTexture();
	material_.setTexture(0, tex);
	float scale = 0.02f;
	buildVertexList(tex, scale, &vertexList_, &indexList_);
}

irr::video::ITexture *CylinderButtonNode::getTexture()
{
	video::ITexture *tex = nullptr;
	if(selected) {
		tex = selectTex_;
	} else {
		tex = normalTex_;
	}
	return tex;
}