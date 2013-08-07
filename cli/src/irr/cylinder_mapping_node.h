// Ŭnicode please 
#pragma once

class CylinderMappingNode : public irr::scene::ISceneNode {
public:
	typedef irr::video::S3DVertex vertex_type;
	typedef std::vector<vertex_type> vertex_list_type;
	typedef std::vector<unsigned short> index_list_type;

public:
	CylinderMappingNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, irr::s32 id);
	virtual ~CylinderMappingNode();

	virtual void OnRegisterSceneNode();
	virtual void render() = 0;
	virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const { return box_; }
	virtual irr::u32 getMaterialCount() const { return 1; }
	virtual irr::video::SMaterial &getMaterial(irr::u32 i) { return material_; }

	// node를 구성하는 내용의 값이 바뀐경우 호출해서 새로운 vertex/index list등을 생성하는 함수
	// 자동으로 호출되도록 바꿀수도 있겠지만 일단은 구조를 간단하게 유지하기 위해서 수동호출
	virtual void rebuild() = 0;

public:
	float radius;

	void buildVertexList(irr::video::ITexture *tex, 
		float scale, 
		vertex_list_type *vertexList, 
		index_list_type *indexList);

protected:
	void renderBasic(const vertex_list_type &vertexList,
		const index_list_type &indexList);

protected:
	irr::core::aabbox3df box_;
	irr::video::SMaterial material_;
};

class CylinderTextureNode : public CylinderMappingNode {
public:
	CylinderTextureNode(irr::scene::ISceneNode *parent, 
		irr::scene::ISceneManager *smgr, 
		irr::s32 id,
		irr::video::ITexture *tex);
	virtual ~CylinderTextureNode();
	virtual void render();
	virtual void rebuild();

private:
	std::vector<vertex_type> vertexList_;
	std::vector<unsigned short> indexList_;
	irr::video::ITexture *tex_;
};

class CylinderButtonNode : public CylinderMappingNode {
public:
	CylinderButtonNode(irr::scene::ISceneNode *parent, 
		irr::scene::ISceneManager *smgr, 
		irr::s32 id,
		const char *normalFile,
		const char *selectFile = nullptr);	
	virtual ~CylinderButtonNode();
	virtual void render();
	virtual void rebuild();

public:
	bool selected;

private:
	irr::video::ITexture *normalTex_;
	irr::video::ITexture *selectTex_;

	std::vector<vertex_type> vertexList_;
	std::vector<unsigned short> indexList_;

	irr::video::ITexture *getTexture();
};
