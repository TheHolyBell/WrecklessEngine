#include "FileDialog.h"
#include <Windows.h>

namespace FileSystem
{

	std::optional<std::string> FileDialog::OpenFile(const char* filter)
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

		std::string cwd = std::filesystem::current_path().string();

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			std::filesystem::current_path(cwd);
			return std::string(ofn.lpstrFile);
		}
		std::filesystem::current_path(cwd);
		return std::nullopt;
	}

	std::optional<std::wstring> FileDialog::OpenFileWide(const wchar_t* filter)
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
	std::optional<std::string> FileDialog::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn = {};       // common dialog box structure
		char szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::nullopt;
	}
	std::optional<std::wstring> FileDialog::SaveFileWide(const wchar_t* filter)
	{
		OPENFILENAMEW ofn = {};       // common dialog box structure
		wchar_t szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameW(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::nullopt;
	}
}