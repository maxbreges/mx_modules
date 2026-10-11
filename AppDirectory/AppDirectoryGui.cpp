#include "mp_sdk_gui2.h"

using namespace gmpi;

class AppDirectoryGui final : public SeGuiInvisibleBase
{
 	void onSetAppDirPath()
	{
		pinAppDirectory = pinAppDirPath;
	}

 	StringGuiPin pinAppDirPath;
 	StringGuiPin pinAppDirectory;

public:
	AppDirectoryGui()
	{
		initializePin( pinAppDirPath, static_cast<MpGuiBaseMemberPtr2>(&AppDirectoryGui::onSetAppDirPath) );
		initializePin( pinAppDirectory );
	}
};

namespace
{
	auto r = Register<AppDirectoryGui>::withId(L"My AppDirectory");
}
