// Ŭnicode please 
#include "stdafx.h"
#include "cylinder_mapping_node.h"
#include "base/lib.h"

using namespace irr;
using namespace std;

namespace irr {;
namespace scene {;

CylinderMappingNode::CylinderMappingNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, irr::s32 id, irr::video::ITexture *tex)
	: ISceneNode(parent, smgr, id),
	radius(10.0f)
{
	//aabb구현 귀찮으니까 일단 culling을 안하는 객체로
	setAutomaticCulling(false);
	box_.reset(core::vector3df(0, 0, 0));

	material_.Wireframe = false;
	//material_.Wireframe = true;
	material_.Lighting = false;
	material_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	material_.setTexture(0, tex);
	//디버깅 삽질 줄일라고 일단 해제. 그거 외에 나중에 원기둥에 광고 붙이는 효과에도 쓰기 위해서 일단 앞뒤 구분 제거
	material_.BackfaceCulling = false;

	//원점에 해당하는곳에 적절히 뭐 하나 찍기. 이거조차 없으면 디버깅하다 망할듯
	//auto cube = smgr->addCubeSceneNode(1, this);
	//cube->setIsDebugObject(true);
	//cube->setMaterialFlag(video::EMF_LIGHTING, false);

	float scale = 0.01f;
	buildVertexList(tex, scale, &vertexList_, &indexList_);
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
	// 어차피 테스트 수준이니까 간단하게 8등분
	const int numSegment = 8;
	vector<float> radList;
	float segmentRad = width / radius / numSegment;
	for(int i = 0 ; i < numSegment ; ++i) {
		float rad = segmentRad * i - halfRad;
		radList.push_back(rad);
	}

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

void CylinderMappingNode::render()
{
	renderBasic(vertexList_, indexList_);
}
}	// namespace scene
}	// namespace irr