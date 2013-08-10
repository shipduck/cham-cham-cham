// Ŭnicode please 
#include "stdafx.h"
#include "sphere_attach_node.h"

using namespace irr;
using namespace std;

SphereAttachNode::SphereAttachNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, irr::s32 id)
	: ISceneNode(parent, smgr, id),
	radius(10)
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
SphereAttachNode::~SphereAttachNode()
{
	if(IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}
	ISceneNode::OnRegisterSceneNode();
}

void SphereAttachNode::OnRegisterSceneNode()
{
}

void SphereAttachNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(material_);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);	

	//테스트용 하나 간단하게
}
