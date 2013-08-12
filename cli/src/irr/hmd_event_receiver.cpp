// Ŭnicode please 
#include "stdafx.h"
#include "hmd_event_receiver.h"
#include "cvars/CVar.h"
#include "util/cvar_key.h"

using namespace irr;

HMDEventReceiver::HMDEventReceiver()
	: supportHMD_(CVarUtils::GetCVarRef<int>(CVAR_ENGINE_DISPLAY_HMD))
{
}
bool HMDEventReceiver::OnEvent(const irr::SEvent &evt)
{
	if(evt.EventType == EET_KEY_INPUT_EVENT) {
		if(evt.KeyInput.PressedDown && evt.KeyInput.Key == KEY_F2) {
			if(supportHMD_ == 0) {
				supportHMD_ = 1;
			} else {
				supportHMD_ = 0;
			}
		}
	}
	return false;
}