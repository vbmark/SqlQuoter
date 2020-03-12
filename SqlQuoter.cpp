#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>

void LTrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
        }));
}

void RTrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

void Trim(std::string &s) {
    LTrim(s);
    RTrim(s);
}

void GetClipboard(std::string& TheText)
{
    HANDLE clip{};
 
    if (OpenClipboard(NULL)) {
        clip = GetClipboardData(CF_TEXT);
    }

    if (clip != 0)
    {
        char* pszText = static_cast<char*>(GlobalLock(clip));
        GlobalUnlock(clip);
        CloseClipboard();
        pszText != 0 ? TheText = pszText : TheText = "Clipboard error";
    }
}

void SetClipboard(const std::string& s) {
    HWND hwnd = GetDesktopWindow();
    OpenClipboard(hwnd);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

void ReplaceAll(std::string& TheText, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while((start_pos = TheText.find(from, start_pos)) != std::string::npos) {
        TheText.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

int main()
{
    std::string TheText;

    GetClipboard(TheText);
    ReplaceAll(TheText, "\t", " ");
    ReplaceAll(TheText, "\r", " ");
    ReplaceAll(TheText, "\n", " ");
    ReplaceAll(TheText, "  ", " ");
    Trim(TheText);
    if (TheText.length() == 0) exit(0);
    ReplaceAll(TheText, " ", "','");

    TheText = "('" + TheText + "')";

    SetClipboard(TheText);

    //std::cout << TheText << std::endl;
    //getchar();
}