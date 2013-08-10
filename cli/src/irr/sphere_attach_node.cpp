// Ŭnicode please 
#include "stdafx.h"
#include "sphere_attach_node.h"

using namespace irr;
using namespace std;

SphereAttachNode::SphereAttachNode(irr::scene::ISceneNode *parent, 
								   irr::scene::ISceneManager *smgr, 
								   irr::s32 id,
								   irr::video::ITexture *tex)
	: ISceneNode(parent, smgr, id),
	scale(0),
	radius(10)
{
	setAutomaticCulling(false);
	box_.reset(core::vector3df(0, 0, 0));

	material_.Wireframe = false;
	//material_.Wireframe = true;
	material_.Lighting = false;
	material_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	material_.setTexture(0, tex);
	//특수효과용으로 사용할때 대비. 지금은 그다지 중요하지 않으니까
	material_.BackfaceCulling = false;
}

SphereAttachNode::~SphereAttachNode()
{
}

void SphereAttachNode::OnRegisterSceneNode()
{
	if(IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}
	ISceneNode::OnRegisterSceneNode();
}