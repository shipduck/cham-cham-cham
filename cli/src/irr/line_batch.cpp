// Ŭnicode please 
#include "stdafx.h"
#include "line_batch.h"

using namespace irr;
using namespace core;
using namespace video;

LineBatch::LineBatch(float thickness)
	: thickness(thickness)
{
	SR_ASSERT(thickness > 0.0f);
	vertexList.reserve(1 << 8);
	indexList.reserve(1 << 8);
}

void LineBatch::add(const vector_3d_type &p1, const vector_3d_type &p2, const color_type &color)
{
	vector3df normal(1, 0, 0);
	vector2df texcoord(0, 0);
	vertex_type v1(p1, normal, color, texcoord);
	vertex_type v2(p2, normal, color, texcoord);

	unsigned short currVertexSize = vertexList.size();
	vertexList.push_back(v1);
	vertexList.push_back(v2);
	indexList.push_back(currVertexSize);
	indexList.push_back(currVertexSize + 1);
}

void LineBatch::add(const vector_2d_type &p1, const vector_2d_type &p2, const color_type &color)
{
	vector3df normal(1, 0, 0);
	vector2df texcoord(0, 0);
	vertex_type v1(vector_3d_type((float)p1.X, (float)p1.Y, 0.0f), normal, color, texcoord);
	vertex_type v2(vector_3d_type((float)p2.X, (float)p2.Y, 0.0f), normal, color, texcoord);

	unsigned short currVertexSize = vertexList.size();
	vertexList.push_back(v1);
	vertexList.push_back(v2);
	indexList.push_back(currVertexSize);
	indexList.push_back(currVertexSize + 1);
}

void LineBatch::draw(irr::video::IVideoDriver *driver, bool depthEnable)
{
	SMaterial material;
	material.Wireframe = false;
	material.Lighting = false;
	material.Thickness = thickness;
	if(depthEnable == false) {
		material.ZBuffer = ECFN_DISABLED;
	}

	driver->setMaterial(material);
	const auto &mat = irr::core::IdentityMatrix;
	driver->setTransform(video::ETS_WORLD, mat);
	
	int primitiveCount = indexList.size() / 2;
	driver->drawVertexPrimitiveList(
		vertexList.data(), 
		vertexList.size(), 
		indexList.data(), 
		primitiveCount,
		video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
}

void LineBatch::draw2D(irr::video::IVideoDriver *driver)
{
	SMaterial material;
	material.Wireframe = false;
	material.Lighting = false;
	material.Thickness = thickness;

	driver->setMaterial(material);
	const auto &mat = irr::core::IdentityMatrix;
	driver->setTransform(video::ETS_WORLD, mat);
	
	int primitiveCount = indexList.size() / 2;
	driver->draw2DVertexPrimitiveList(
		vertexList.data(), 
		vertexList.size(), 
		indexList.data(), 
		primitiveCount,
		video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
}

void LineBatch::clear()
{
	vertexList.clear();
	indexList.clear();
}
