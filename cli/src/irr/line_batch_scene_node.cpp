// Ŭnicode please 
#include "stdafx.h"
#include "line_batch_scene_node.h"

using namespace irr;
using namespace video;
using namespace core;

LineBatchSceneNode::LineBatchSceneNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, s32 id)
	: scene::ISceneNode(parent, smgr, id)
{
	setAutomaticCulling(irr::scene::EAC_OFF);
	this->setIsDebugObject(true);
	material_.Wireframe = false;
	material_.Lighting = false;

	box_.reset(vector3df(0, 0, 0));
}

void LineBatchSceneNode::OnRegisterSceneNode()
{
	if(IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}
	ISceneNode::OnRegisterSceneNode();
}

void LineBatchSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(material_);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	
	driver->drawVertexPrimitiveList(
		batch3D_.vertexList.data(), 
		batch3D_.vertexList.size(), 
		batch3D_.indexList.data(), 
		batch3D_.indexList.size() / 2, 
		video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);

	driver->draw2DVertexPrimitiveList(
		batch2D_.vertexList.data(), 
		batch2D_.vertexList.size(), 
		batch2D_.indexList.data(), 
		batch2D_.indexList.size() / 2, 
		video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
}

void LineBatchSceneNode::addLine(const vector_3d_type &p1, const vector_3d_type &p2, const color_type &color)
{
	vector3df normal(1, 0, 0);
	vector2df texcoord(0, 0);
	vertex_type v1(p1, normal, color, texcoord);
	vertex_type v2(p2, normal, color, texcoord);

	batch3D_.add(p1, p2, color);
}

void LineBatchSceneNode::addLine(const vector_2d_type &p1, const vector_2d_type &p2, const color_type &color)
{
	batch2D_.add(p1, p2, color);
}

void LineBatchSceneNode::clear()
{
	batch2D_.clear();
	batch3D_.clear();
}

void LineBatchSceneNode::setThickness(float val)
{
	batch3D_.thickness = val;
	batch2D_.thickness = val; 
	material_.Thickness = val;
}