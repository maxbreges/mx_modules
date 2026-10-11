#include "mp_sdk_audio.h"

using namespace gmpi;

class AppDirectory final : public MpBase2
{
	StringInPin pinResolveFilename;//
	StringOutPin pinToParam;		
	StringOutPin pinAppDirPathOut;

public:
	AppDirectory()
	{	
		initializePin(pinResolveFilename);
		initializePin(pinToParam);
		initializePin( pinAppDirPathOut );
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if(pinResolveFilename.isUpdated() )
		{
			std::wstring filename = pinResolveFilename.getValue() + L".txt";
			wchar_t fullFilename[1024];
			getHost()->resolveFilename(filename.c_str(), sizeof(fullFilename) / sizeof(fullFilename[0]), fullFilename);
			pinAppDirPathOut = pinToParam = get_directory(fullFilename);
		}
	}

	std::wstring get_directory(const std::wstring& path) //helper
	{
		size_t sepPos = path.find_last_of(L"/\\");
		if (sepPos != std::string::npos)
			return path.substr(0, sepPos + 1);
		return L"";
	}
};

namespace
{
	auto r = Register<AppDirectory>::withId(L"My AppDirectory");
}
