// Ŭnicode please 
#include "stdafx.h"
#include "console_func.h"
#include "base/lib.h"
#include "base/string_util.h"

namespace console {;
bool driverInfo(const std::vector<std::string> &args)
{
	if(Lib::device) {
		std::string msg = " Irrlicht Version : ";
		msg += Lib::device->getVersion();
		Lib::printf(msg.c_str());

		msg = " OS Version : ";
		auto osInfo = Lib::device->getOSOperator()->getOperatingSystemVersion();
		msg += osInfo.c_str();
		Lib::printf(msg.c_str());

		msg = " Display Driver : ";
		msg += StringUtil::wstring2string(Lib::driver->getName());
		Lib::printf(msg.c_str());

		return true;
	} else {
		Lib::warning("No valid irrlicht device detected!");
		return false;
	}
}
}	// namespace console