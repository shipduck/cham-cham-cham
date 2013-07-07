// Ŭnicode please 
#pragma once

struct KeyMapData {
	//keyboard event
	irr::EKEY_CODE keyCode;
};

class KeyMapping {
public:
	enum {
		kPrimary,
		kSecondary,
		kKeyBindCount,
	};
	enum {
		kForwardKey,
		kBackwardKey,
		kLeftKey,
		kRightKey,
	};

public:
	KeyMapping();

public:
	const std::vector<KeyMapData> &getForwardKeyList() const { return forwardKeyList_; }
	const std::vector<KeyMapData> &getBackwardKeyList() const { return backwardKeyList_; }
	const std::vector<KeyMapData> &getLeftKeyList() const { return leftKeyList_; }
	const std::vector<KeyMapData> &getRightKeyList() const { return rightKeyList_; }

private:
	std::vector<KeyMapData> forwardKeyList_;
	std::vector<KeyMapData> backwardKeyList_;
	std::vector<KeyMapData> leftKeyList_;
	std::vector<KeyMapData> rightKeyList_;
};
