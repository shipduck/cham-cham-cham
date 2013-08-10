// Ŭnicode please 
#include "stdafx.h"
#include "sprite_scene_node.h"

using namespace std;

namespace irr {;
namespace scene {;

SpriteSceneNode::SpriteSceneNode(irr::scene::ISceneNode* parent, 
	irr::scene::ISceneManager* mgr, 
	s32 id,
	irr::video::ITexture *tex)
	: ISceneNode(parent, mgr, id)
{
	//aabb구현 귀찮으니까 일단 culling을 안하는 객체로
	setAutomaticCulling(false);
	box_.reset(core::vector3df(0, 0, 0));

	video::SColor white(255, 255, 255, 255);

	indices_[0] = 0;
	indices_[1] = 2;
	indices_[2] = 1;
	indices_[3] = 0;
	indices_[4] = 3;
	indices_[5] = 2;

	vertices_[0].TCoords.set(1.0f, 1.0f);
	vertices_[0].Color = white;

	vertices_[1].TCoords.set(1.0f, 0.0f);
	vertices_[1].Color = white;

	vertices_[2].TCoords.set(0.0f, 0.0f);
	vertices_[2].Color = white;

	vertices_[3].TCoords.set(0.0f, 1.0f);
	vertices_[3].Color = white;

	material_.Wireframe = false;
	material_.Lighting = false;
	material_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	material_.setTexture(0, tex);
	material_.BackfaceCulling = false;
}
SpriteSceneNode::~SpriteSceneNode()
{

}

void SpriteSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);
	ISceneNode::OnRegisterSceneNode();
}
void SpriteSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver) {
		return;
	}

	core::vector3df normal(0, 0, -1);
	for(size_t i = 0 ; i < vertices_.size() ; ++i) {
		vertices_[i].Normal = normal;
	}
	/* Vertices are:
	2--1
	|\ |
	| \|
	3--0
	*/
	core::vector3df horizontal(size_.Width, 0, 0);
	core::vector3df vertical(0, size_.Height, 0);

	vertices_[0].Pos = horizontal - vertical;
	vertices_[1].Pos = horizontal + vertical;
	vertices_[2].Pos = -horizontal + vertical;
	vertices_[3].Pos = -horizontal - vertical;

	driver->setMaterial(material_);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);	

	driver->drawIndexedTriangleList(
		vertices_.data(), 
		vertices_.size(), 
		indices_.data(), 
		2);
}

}	// namespace scene
}	// namespace irr