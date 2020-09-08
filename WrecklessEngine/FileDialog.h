#pragma once

#include <optional>
#include <string>
#include <filesystem>

namespace FileSystem
{
	class FileDialog
	{
	public:
		static std::optional<std::string> ShowDialog(const char* filter = "All Files(*.*)\0*.*\0");
		static std::optional<std::wstring> ShowDialogWide(const wchar_t* filter = L"All Files(*.*)\0*.*\0");
	};
}