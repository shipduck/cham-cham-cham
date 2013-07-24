// Ŭnicode please 
#include "stdafx.h"
#include "message.h"

SDestroyMessage SDestroyMessage::create(int objId) {
	SDestroyMessage msg;
	msg.ObjId = objId;
	return msg;
}
