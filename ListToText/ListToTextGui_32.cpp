#include "mp_sdk_gui2.h"
#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>

using namespace gmpi;

class ListToTextGui final : public SeGuiInvisibleBase
{
    void onSetChoice()
    {
        updateItemText();
    }

    void onSetItemList()
    {
        updateItemText();
    }

    IntGuiPin pinChoice;
    StringGuiPin pinItemList;
    StringGuiPin pinItemText;

    // Helper function to split a wstring based on a wchar_t delimiter
    std::vector<std::wstring> splitString(const std::wstring& s, wchar_t delimiter)
    {
        std::vector<std::wstring> tokens;
        std::wstringstream ss(s);
        std::wstring item;
        while (std::getline(ss, item, delimiter))
        {
            tokens.push_back(item);
        }
        return tokens;
    }

    void updateItemText()
    {
        // Use pinItemList directly as a wstring
        std::wstring listW = pinItemList;

        auto items = splitString(listW, L',');

        int choiceIndex = static_cast<int>(pinChoice);

        if (choiceIndex >= 0 && choiceIndex < static_cast<int>(items.size()))
        {
            pinItemText = items[choiceIndex]; // pinItemText is wstring
        }
        else
        {
            pinItemText = L"";
        }
    }

public:
    ListToTextGui()
    {
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&ListToTextGui::onSetChoice));
        initializePin(pinItemList, static_cast<MpGuiBaseMemberPtr2>(&ListToTextGui::onSetItemList));
        initializePin(pinItemText);
    }
};

namespace
{
    auto r = Register<ListToTextGui>::withId(L"ListToText");
}