// Ŭnicode please 
#include "stdafx.h"
#include "key_mapping.h"
#include "util/console_func.h"

using namespace irr;

std::ostream &operator<<( std::ostream &stream, KeyMapData& mKeyMap ) {
    stream << CVarUtils::CVarSpc() << "KEY(" << mKeyMap.keyCode << ")";
    return stream;
}

std::istream &operator>>( std::istream &stream, KeyMapData& mKeyMap ) {
    char buf;
    std::string strBuf;
    uint8_t state = 0;
    while(stream >> buf) {
        if ((state == 0 && buf == 'K')
            || (state == 1 && buf == 'E')
            || (state == 2 && buf == 'Y')
            || (state == 3 && buf == '(')) {
            state++;
		} else if (state == 4 && isdigit(buf)) {
            strBuf += buf;
        } else if (state == 4 && buf == ')') {
            break;
        }
    }
    mKeyMap.keyCode = static_cast<irr::EKEY_CODE>(boost::lexical_cast<int>(strBuf));
    return stream;
}

typedef boost::bimap<KeyMapping::eKeyType, std::string> keyTypeStringMap_t;
static keyTypeStringMap_t g_keyTypeStringMap = boost::assign::list_of<keyTypeStringMap_t::relation>
    (KeyMapping::kActionKey, "KEY_ACTION")
    (KeyMapping::kCancleKey, "KEY_CANCLE")
    (KeyMapping::kConfirmKey, "KEY_CONFIRM")
    (KeyMapping::kOptionKey, "KEY_OPTION")
    (KeyMapping::kLeftTriggerKey, "KEY_LEFT_TRIGGER")
    (KeyMapping::kRightTriggerKey, "KEY_RIGHT_TRIGGER")
    (KeyMapping::kLeftFunctionKey, "KEY_LEFT_FUNCTION")
    (KeyMapping::kRightFunctionKey, "KEY_RIGHT_FUNCTION")
    (KeyMapping::kStartKey, "KEY_START")
    (KeyMapping::kSelectKey, "KEY_SELECT")
    (KeyMapping::kLeftKey, "KEY_LEFT")
    (KeyMapping::kRightKey, "KEY_RIGHT")
    (KeyMapping::kForwardKey, "KEY_FORWARD")
    (KeyMapping::kBackwardKey, "KEY_BACKWARD")
    (KeyMapping::kUndefinedKey, "KEY_UNDEFINED");

std::ostream &operator<<( std::ostream &stream, KeyMapping::eKeyType& keyType) {
    stream << g_keyTypeStringMap.left.find(keyType)->second;
    return stream;
}

std::istream &operator>>( std::istream &stream, KeyMapping::eKeyType& keyType) {
    std::string buf;
    stream >> buf;
    keyTypeStringMap_t::right_const_iterator itr = g_keyTypeStringMap.right.find(buf);
    if (itr != g_keyTypeStringMap.right.end()) {
        keyType = itr->second;
    }
    return stream;
}

KeyMapping::KeyMapping()
{
}

void KeyMapping::startUp()
{
	// keyboard
    
	//forward
	keyMapList_t forwardKeyMap;
	forwardKeyMap.push_back(KeyMapData(irr::KEY_UP));
	forwardKeyMap.push_back(KeyMapData(irr::KEY_KEY_W));
    keyBoardKeyMapList_[kForwardKey] = forwardKeyMap;
    
    //backward
	keyMapList_t backwardKeyMap;
	backwardKeyMap.push_back(KeyMapData(irr::KEY_DOWN));
	backwardKeyMap.push_back(KeyMapData(irr::KEY_KEY_S));
	keyBoardKeyMapList_[kBackwardKey] = backwardKeyMap;
    
    //left
	keyMapList_t leftKeyMap;
	leftKeyMap.push_back(KeyMapData(irr::KEY_LEFT));
	leftKeyMap.push_back(KeyMapData(irr::KEY_KEY_A));
	keyBoardKeyMapList_[kLeftKey] = leftKeyMap;
    
    //right
	keyMapList_t rightKeyMap;
	rightKeyMap.push_back(KeyMapData(irr::KEY_RIGHT));
	rightKeyMap.push_back(KeyMapData(irr::KEY_KEY_D));
	keyBoardKeyMapList_[kRightKey] = rightKeyMap;
    
    CVarUtils::AttachCVar("input.keyboard.forward", &keyBoardKeyMapList_[kForwardKey]);
    CVarUtils::AttachCVar("input.keyboard.backward", &keyBoardKeyMapList_[kBackwardKey]);
    CVarUtils::AttachCVar("input.keyboard.left", &keyBoardKeyMapList_[kLeftKey]);
    CVarUtils::AttachCVar("input.keyboard.right", &keyBoardKeyMapList_[kRightKey]);
    CVarUtils::AttachCVar("input.keyboard.leftTrigger", &keyBoardKeyMapList_[kLeftTriggerKey]);
    CVarUtils::AttachCVar("input.keyboard.rightTrigger", &keyBoardKeyMapList_[kRightTriggerKey]);
    CVarUtils::AttachCVar("input.keyboard.leftFunction", &keyBoardKeyMapList_[kLeftFunctionKey]);
    CVarUtils::AttachCVar("input.keyboard.rightFunction", &keyBoardKeyMapList_[kRightFunctionKey]);
    CVarUtils::AttachCVar("input.keyboard.select", &keyBoardKeyMapList_[kSelectKey]);
    CVarUtils::AttachCVar("input.keyboard.start", &keyBoardKeyMapList_[kStartKey]);
    CVarUtils::AttachCVar("input.keyboard.action", &keyBoardKeyMapList_[kActionKey]);
    CVarUtils::AttachCVar("input.keyboard.option", &keyBoardKeyMapList_[kOptionKey]);
    CVarUtils::AttachCVar("input.keyboard.confirm", &keyBoardKeyMapList_[kConfirmKey]);
    CVarUtils::AttachCVar("input.keyboard.cancle", &keyBoardKeyMapList_[kCancleKey]);
    
    // joystick
    joystickKeyMap_[kButton1] = kActionKey;
    joystickKeyMap_[kButton2] = kOptionKey;
    joystickKeyMap_[kButton3] = kConfirmKey;
    joystickKeyMap_[kButton4] = kCancleKey;
    joystickKeyMap_[kLB]      = kLeftFunctionKey;
    joystickKeyMap_[kRB]      = kRightFunctionKey;
    joystickKeyMap_[kStart]   = kStartKey;
    joystickKeyMap_[kBack]    = kSelectKey;
    
    CVarUtils::AttachCVar("input.joystick.leftButton", &joystickKeyMap_[kLB]);
    CVarUtils::AttachCVar("input.joystick.rightButton", &joystickKeyMap_[kRB]);
    CVarUtils::AttachCVar("input.joystick.button_1", &joystickKeyMap_[kButton1]);
    CVarUtils::AttachCVar("input.joystick.button_2", &joystickKeyMap_[kButton2]);
    CVarUtils::AttachCVar("input.joystick.button_3", &joystickKeyMap_[kButton3]);
    CVarUtils::AttachCVar("input.joystick.button_4", &joystickKeyMap_[kButton4]);
    CVarUtils::AttachCVar("input.joystick.startButton", &joystickKeyMap_[kStart]);
    CVarUtils::AttachCVar("input.joystick.backButton", &joystickKeyMap_[kBack]);

	std::vector<std::string> varLoad;
	varLoad.push_back("input");
	console::load(varLoad);
}
void KeyMapping::shutDown()
{
	//변수 저장은 어차피 console 관련에서 한방에 처리하니까 굳이 따로 할필요없다
}