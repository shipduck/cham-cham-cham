// Ŭnicode please 
#pragma once

class Sequence;
class DemoSequenceHelper {
public:
	DemoSequenceHelper(Sequence *seq);
	
	//void loadSampleCollisionMap();
	void loadSampleMap();
	irr::scene::ICameraSceneNode *createFPSCamera();
	//irr::scene::ICameraSceneNode *createCollisionFPSCamera(irr::scene::ITriangleSelector *selector);

	const std::vector<irr::SKeyMap> getKeyMapData() const;

	static irr::core::vector3df getDefaultCamPos();
	static irr::core::vector3df getDefaultCamTarget();

private:
	Sequence *sequence_;

	irr::scene::IMeshSceneNode *q3node_;
	
};