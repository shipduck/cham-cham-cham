// Ŭnicode please
#pragma once

#include "CVars/cvars_tinyxml.h"
#include "boost/lexical_cast.hpp"

struct KeyMapData {
	//keyboard event
	KeyMapData() : keyCode(irr::KEY_RETURN) {}
	KeyMapData(irr::EKEY_CODE code) : keyCode(code) {}
	irr::EKEY_CODE keyCode;
};

std::ostream &operator<<( std::ostream &stream, KeyMapData& mKeyMap );
std::istream &operator>>( std::istream &stream, KeyMapData& mKeyMap );

class KeyMapping {
public:
	enum {
		kPrimary,
		kSecondary,
		kKeyBindCount,
	};
    /*
     XBox 360 controller
     Action : 0x2000
     Option : 0x4000
     Cancle : 0x1000
     Confirm : 0x0800
     Select : 0x0020
     Start : 0x0010
     LeftFunction : 0x0100
     RightFunction : 0x0200
     */
    enum eJoystickButtonType:uint32_t {
        kLB = 0x0100,
        kRB = 0x0200,
        kStart = 0x0010,
        kBack = 0x0020,
        kButton1 = 0x2000,
        kButton2 = 0x4000,
        kButton3 = 0x0800,
        kButton4 = 0x1000
    };
	enum eKeyType:uint8_t {
        kUndefinedKey,
        kActionKey,
        kCancleKey,
        kConfirmKey,
        kOptionKey,
        kLeftTriggerKey,
        kRightTriggerKey,
        kLeftFunctionKey,
        kRightFunctionKey,
        kStartKey,
        kSelectKey,
        kLeftKey,
        kRightKey,
        kForwardKey,
        kBackwardKey,
        kKeyTypeCount
    };
    
    typedef std::vector<KeyMapData> keyMapList_t;
    typedef std::map<eKeyType, keyMapList_t> keyMap_t;
    typedef std::map<eJoystickButtonType, eKeyType> joystickMap_t;

public:
	KeyMapping();

	void startUp();
	void shutDown();

public:
    const keyMapList_t &getKeyList(const eKeyType keyType) const { return keyBoardKeyMapList_.at(keyType); }
    const keyMap_t &getKeyMap(void) const { return keyBoardKeyMapList_; }
    const joystickMap_t &getJoystickKeyMap(void) const { return joystickKeyMap_; }

private:
    keyMap_t keyBoardKeyMapList_;
    joystickMap_t joystickKeyMap_;
};

std::ostream &operator<<( std::ostream &stream, KeyMapping::eKeyType& keyType);
std::istream &operator>>( std::istream &stream, KeyMapping::eKeyType& keyType);