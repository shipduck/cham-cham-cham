// Ŭnicode please 
#pragma once

class Sequence;
class DemoSequenceHelper {
public:
	DemoSequenceHelper(Sequence *seq);
	
	void loadSampleCollisionMap();

private:
	Sequence *sequence_;
};