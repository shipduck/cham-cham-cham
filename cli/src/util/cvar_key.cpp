// Ŭnicode please 
#include "stdafx.h"
#include "cvar_key.h"
#include "console_func.h"

using namespace std;

void initConsoleFunction()
{
	CVarUtils::CreateCVar("driver_info", console::driverInfo, "Display Irrlicht Driver Info");	
	CVarUtils::CreateCVar("play_bgm", console::playBGM, "Play test bgm");
	CVarUtils::CreateCVar("save", console::save, "Save the CVars to a file");
	CVarUtils::CreateCVar("load", console::load, "Load CVars from a file");
}

void initConsoleVar()
{
	//console에서 사용할 변수는 전부 미리 획득하고 값을 로딩해야 다른곳에서 속편하게 쓸수있을거같다
	//이렇게 구성하지 않는다면 위치에 따라서 로딩을 여러번해야되는데 그건 그거대로 귀찮고...
	//게다가 CVarCreate는 1번만 호출 가능하니까 한곳에서 몰아서 Create하고 다른곳에서는 CvarRefGet로만 쓸수있도록하자

	// engine
	CVarUtils::CreateCVar<int>(CVAR_ENGINE_DISPLAY_FULLSCREEN, 0);
	CVarUtils::CreateCVar<int>(CVAR_ENGINE_DISPLAY_WIDTH, 1280);
	CVarUtils::CreateCVar<int>(CVAR_ENGINE_DISPLAY_HEIGHT, 800);
	CVarUtils::CreateCVar<int>(CVAR_ENGINE_DISPLAY_SHOW_FPS, 1);
	
	//hmd
	CVarUtils::CreateCVar<int>(CVAR_HMD_H_RESOLUTION, 1280);
	CVarUtils::CreateCVar<int>(CVAR_HMD_V_RESOLUTION, 800);
	CVarUtils::CreateCVar<float>(CVAR_HMD_H_SCREEN_SIZE, 0.14976f);
	CVarUtils::CreateCVar<float>(CVAR_HMD_V_SCREEN_SIZE, 0.0936f);
	CVarUtils::CreateCVar<float>(CVAR_HMD_INTERPUPILLARY_DISTANCE, 0.064f);
	CVarUtils::CreateCVar<float>(CVAR_HMD_LENS_SEPARATION_DISTANCE, 0.064f);
	CVarUtils::CreateCVar<float>(CVAR_HMD_EYE_TO_SCREEN_DISTANCE, 0.041f);
	CVarUtils::CreateCVar<float>(CVAR_HMD_DISTORTION_K_1, 1.0f);
	CVarUtils::CreateCVar<float>(CVAR_HMD_DISTORTION_K_2, 0.22f);
	CVarUtils::CreateCVar<float>(CVAR_HMD_DISTORTION_K_3, 0.24f);
	CVarUtils::CreateCVar<float>(CVAR_HMD_DISTORTION_K_4, 0.0f);

	//console, engine는 내가 직접 개입할거같지 않으니까 그냥 해당 클래스에서 선언한거 갖다쓰자

	std::vector<std::string> varLoad;
	varLoad.push_back("hmd");
	varLoad.push_back("engine");
	console::load(varLoad);
}

void startUpCVar()
{
	initConsoleFunction();
	initConsoleVar();
}
void shutDownCVar()
{
	vector<string> cvarNamespaceList;
	cvarNamespaceList.push_back("console");
	cvarNamespaceList.push_back("hmd");
	cvarNamespaceList.push_back("engine");
	cvarNamespaceList.push_back("script");
	console::save(cvarNamespaceList);
}