// Ŭnicode please 
#include "stdafx.h"
#include "game_console.h"
#include "util/debug_draw_manager.h"

IC_Console gConsole;

void drawConsoleCaptions(irr::IrrlichtDevice *device)
{
	auto font = gNormalFont14;
	if(font == nullptr) {
		font = device->getGUIEnvironment()->getBuiltInFont();
	}
	//initialize the caption line height
	SR_ASSERT(font != nullptr);
	static int captionHeight = -1;
	if(captionHeight < 0) {
		captionHeight = font->getDimension(L"X").Height + 2;
	}

	static array<irr::core::stringw> captionText;
	if(captionText.size() == 0) {
		//initialize the caption array
		captionText.push_back(L"IrrConsole : Quake Style Drop Down Console Demo For Irrlicht");
		captionText.push_back(L"Author : Saurav Mohapatra (mohaps@gmail.com)");
		captionText.push_back(L"");
		captionText.push_back(L"HELP TEXT:");
		captionText.push_back(L"============================================================");
		captionText.push_back(L"     Press the tilde (~) key to toggle console");
		captionText.push_back(L"     Use the UP/DN arrow keys to access command history");
		captionText.push_back(L"     Type commands at the prompt");
		captionText.push_back(L"     To execute a command put \\ in front");
		captionText.push_back(L"     e.g. \\list");
		captionText.push_back(L"     use the \"list\" or \"help\" commands to find out more");
		captionText.push_back(L"     to start try the command \"\\help show_node\");");
		captionText.push_back(L"============================================================");
		captionText.push_back(L" ");
		captionText.push_back(L"TO QUIT THIS DEMO PRESS ESCAPE KEY");
	}

	static const irr::video::SColor fontColor(200, 200, 200, 200);
	dimension2d<u32> screenDim = device->getVideoDriver()->getScreenSize();
	rect<s32> lineRect(10,10,screenDim.Width, screenDim.Height);
	for(u32 i = 0; i < captionText.size(); i++)
	{
		font->draw(captionText[i].c_str(),lineRect,fontColor);
		lineRect.UpperLeftCorner.Y += captionHeight;
		lineRect.LowerRightCorner.Y += captionHeight;
	}
}

bool onConsoleEvent(const irr::SEvent &event)
{
	if(event.EventType == irr::EET_KEY_INPUT_EVENT ) {
		if(event.KeyInput.PressedDown) {
			if(event.KeyInput.Key == irr::KEY_ESCAPE) {
				if(gConsole.isVisible()) {
					gConsole.setVisible(false);
					return true;
				}
			} else if(event.KeyInput.Key == IC_Console::IC_KEY_TILDE) {
				if(!gConsole.isVisible()) {
					gConsole.setVisible(true);
					return true;
				} else if(!event.KeyInput.Control) {
					gConsole.setVisible(false);
					return true;
				}
			} if(gConsole.isVisible()) {
				gConsole.handleKeyPress(event.KeyInput.Char, event.KeyInput.Key,event.KeyInput.Shift, event.KeyInput.Control);
				return true;
			}
		}
	} else if(event.EventType == irr::EET_LOG_TEXT_EVENT) {
		gConsole.logMessage_ANSI(event.LogEvent.Level,event.LogEvent.Text);
		return true;
	} else if(event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		return gConsole.isVisible();
	}
	return false;
}