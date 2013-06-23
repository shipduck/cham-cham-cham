// Ŭnicode please 
#include "stdafx.h"
#include "message.h"

DestroyMessage DestroyMessage::create(int objId) {
	DestroyMessage msg;
	msg.ObjId = objId;
	return msg;
}
