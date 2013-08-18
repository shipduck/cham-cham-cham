// Ŭnicode please 
#include "stdafx.h"
#include "console_func.h"
#include "base/lib.h"
#include "base/string_util.h"
#include "util/audio_manager.h"
#include "res.h"
#include "SimpleAudioEngine.h"

namespace console {;
bool driverInfo(const std::vector<std::string> &args)
{
	if(Lib::device) {
		std::string msg = " Irrlicht Version : ";
		msg += Lib::device->getVersion();
		Lib::printf(msg);

		msg = " OS Version : ";
		auto osInfo = Lib::device->getOSOperator()->getOperatingSystemVersion();
		msg += osInfo.c_str();
		Lib::printf(msg);

		msg = " Display Driver : ";
		msg += StringUtil::wstring2string(Lib::driver->getName());
		Lib::printf(msg);

		return true;
	} else {
		Lib::warning("No valid irrlicht device detected!");
		return false;
	}
}

bool playBGM(const std::vector<std::string> &args)
{
	if(Lib::audio->isSupport()) {
		Lib::audio->playBackgroundMusic(res::theme::SLIDERGIRLS_LONG_WAV);
		return true;
	} else {
		return false;
	}	
	return true;
}

bool save(const std::vector<std::string> &args)
{
	std::string sFile = "cvars.xml";
	const std::vector<std::string> &vAcceptedSubstrings = args;

	printf( "Saving cvars to \"%s\".", sFile.c_str() );
	if( !CVarUtils::Save( sFile, vAcceptedSubstrings ) ) {
		printf( "Error saving file.\n" );
		return false;
	}

	return true;
}

bool load(const std::vector<std::string> &args)
{
	std::string sFile = "cvars.xml";
	const std::vector<std::string> &vAcceptedSubstrings = args;

	printf( "Loading file \"%s\".\n", sFile.c_str() );
	if( !CVarUtils::Load( sFile, vAcceptedSubstrings ) ) {
		printf( "Error: Could not load \"%s\".", sFile.c_str() );
	}
	return true;
}

bool restartGame(const std::vector<std::string> &args)
{
	
	return true;
}

}	// namespace console