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
	Material.Wireframe = false;
	Material.Lighting = false;

	Box.reset(vector3df(0, 0, 0));

	Vertex3DList.reserve(1 << 8);
	Index3DList.reserve(1 << 8);
	Vertex2DList.reserve(1 << 8);
	Index2DList.reserve(1 << 8);
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

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	
	driver->drawVertexPrimitiveList(
		Vertex3DList.data(), 
		Vertex3DList.size(), 
		Index3DList.data(), 
		Index3DList.size() * 0.5, 
		video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);

	driver->draw2DVertexPrimitiveList(
		Vertex2DList.data(), 
		Vertex2DList.size(), 
		Index2DList.data(), 
		Index2DList.size() * 0.5, 
		video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
}

void LineBatchSceneNode::addLine(const vector_3d_type &p1, const vector_3d_type &p2, const color_type &color)
{
	vector3df normal(1, 0, 0);
	vector2df texcoord(0, 0);
	vertex_type v1(p1, normal, color, texcoord);
	vertex_type v2(p2, normal, color, texcoord);

	unsigned short currVertexSize = Vertex3DList.size();
	Vertex3DList.push_back(v1);
	Vertex3DList.push_back(v2);
	Index3DList.push_back(currVertexSize);
	Index3DList.push_back(currVertexSize + 1);
}

void LineBatchSceneNode::addLine(const vector_2d_type &p1, const vector_2d_type &p2, const color_type &color)
{
	vector3df normal(1, 0, 0);
	vector2df texcoord(0, 0);
	vertex_type v1(vector_3d_type((float)p1.X, (float)p1.Y, 0.0f), normal, color, texcoord);
	vertex_type v2(vector_3d_type((float)p2.X, (float)p2.Y, 0.0f), normal, color, texcoord);

	unsigned short currVertexSize = Vertex2DList.size();
	Vertex2DList.push_back(v1);
	Vertex2DList.push_back(v2);
	Index2DList.push_back(currVertexSize);
	Index2DList.push_back(currVertexSize + 1);
}

void LineBatchSceneNode::clear()
{
	Vertex3DList.clear();
	Index3DList.clear();
	Vertex2DList.clear();
	Index2DList.clear();
}
