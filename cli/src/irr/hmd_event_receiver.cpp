// Ŭnicode please 
#include "stdafx.h"
#include "hmd_event_receiver.h"

using namespace irr;

HMDEventReceiver hmdEventReceiverLocal;
HMDEventReceiver *g_hmdEventReceiver = &hmdEventReceiverLocal;

bool HMDEventReceiver::OnEvent(const irr::SEvent &evt)
{
	if(evt.EventType == EET_KEY_INPUT_EVENT) {
		if(evt.KeyInput.PressedDown && evt.KeyInput.Key == KEY_F2) {
			supportHMD_ = !supportHMD_;
		}
	}
	return false;
}