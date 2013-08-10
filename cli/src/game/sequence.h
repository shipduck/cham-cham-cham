// Ŭnicode please 
#pragma once

class Sequence {
public:
	Sequence();
	virtual ~Sequence();

	virtual void update(int ms) = 0;
	virtual void draw();

	//draw 하기 전에 확실히 수행되어야한다고 보장되는 작업이 있을때 사용
	virtual void preDraw() = 0;
	virtual void postDraw() = 0;

private:
	void drawAllNormal(irr::scene::ISceneManager *smgr);
	void drawAllStereo(irr::scene::ISceneManager *smgr);	
};
