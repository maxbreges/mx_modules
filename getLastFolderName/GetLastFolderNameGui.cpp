#include "mp_sdk_gui2.h"

using namespace gmpi;

class GetLastFolderNameGui final : public SeGuiInvisibleBase
{
 	void onSetText()
	{
        pinText2 = getLastFolderName(pinText);
        pinConcatenated = pinText2;
	}


 	StringGuiPin pinText;
 	StringGuiPin pinText2;
 	StringGuiPin pinConcatenated;

public:
	GetLastFolderNameGui()
	{
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&GetLastFolderNameGui::onSetText) );
		initializePin( pinText2 );
		initializePin( pinConcatenated );
	}

    //Helper
    std::string getLastFolderName(const std::string& path)
    {
        // Remove trailing slashes
        size_t endPos = path.size();
        while (endPos > 0 && (path[endPos - 1] == '/' || path[endPos - 1] == '\\'))
            --endPos;

        // Find the last slash before endPos
        size_t lastSlashPos = path.rfind('/', endPos - 1);
#if defined(_WIN32)
        size_t lastBackslashPos = path.rfind('\\', endPos - 1);
        lastSlashPos = std::max<int>(lastSlashPos, lastBackslashPos);
#endif

        if (lastSlashPos == std::string::npos)
            return ""; // no slash found, no folder

        // Extract the folder name after last slash
        return path.substr(lastSlashPos + 1, endPos - lastSlashPos);
    }

};

namespace
{
	auto r = Register<GetLastFolderNameGui>::withId(L"My getLastFolderName");
}
