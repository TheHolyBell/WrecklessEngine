#include "FileDialog.h"
#include <Windows.h>

namespace FileSystem
{

	std::optional<std::string> FileDialog::ShowDialog(const char* filter)
	{
		OPENFILENAMEA ofn = {};
		TCHAR szFile[260] = {};

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return std::string(ofn.lpstrFile);

		return std::nullopt;
	}

	std::optional<std::wstring> FileDialog::ShowDialogWide(const wchar_t* filter)
	{
		OPENFILENAMEW ofn = {};
		wchar_t szFile[260] = {};

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameW(&ofn) == TRUE)
			return std::wstring(ofn.lpstrFile);

		return std::nullopt;
	}
}