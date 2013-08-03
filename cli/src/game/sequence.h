// Ŭnicode please 
#pragma once

class Sequence {
public:
	Sequence();
	virtual ~Sequence();

	virtual void update(int ms) = 0;
	virtual void draw();

	virtual void preDraw();
private:
	void drawAllNormal(irr::scene::ISceneManager *smgr);
	void drawAllStereo(irr::scene::ISceneManager *smgr);	
};
