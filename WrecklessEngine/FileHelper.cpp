#include "FileHelper.h"
#include <filesystem>
#include <sstream>

namespace FileSystem
{
	FileHelper::FileException::FileException(const char* file, int line, const std::string& msg)
		: Exceptions::WrecklessException(file, line), m_Message(msg)
	{
	}
	const char* FileHelper::FileException::what() const
	{
		std::ostringstream oss;
		oss << Exceptions::WrecklessException::what();
		oss << std::endl << m_Message;
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* FileHelper::FileException::GetType() const noexcept
	{
		return "FileException";
	}
	bool FileHelper::Exists(const std::string& path)
	{
		return std::filesystem::exists(path);
	}
	void FileHelper::CreateFolder(const std::string& path)
	{
		if (std::filesystem::exists(path))
			throw FileException(__FILE__, __LINE__, path + " - folder already exists");
		std::filesystem::create_directory(path);
	}
	void FileHelper::RemoveFile(const std::string& path)
	{
		if (Exists(path))
			std::remove(path.c_str());
		else
			throw FileException(__FILE__, __LINE__, path + " - File on this path doesn't exists");
	}
	std::string FileHelper::GetCurrentWorkingDirectory()
	{
		return std::filesystem::current_path().string();
	}
	bool FileHelper::IsFile(const std::string& path)
	{
		return std::filesystem::path{ path }.has_extension();
	}
	bool FileHelper::IsDirectory(const std::string& path)
	{
		return std::filesystem::is_directory(path);
	}
	std::string FileHelper::GetDirectoryFromPath(const std::string& path)
	{
		std::filesystem::path _path(path);

		if (std::filesystem::is_directory(path))
			return path;

		return GetParentDirectory(path);
	}
	std::string FileHelper::GetFileNameFromPath(const std::string& path)
	{
		std::filesystem::path _path(path);
		return _path.filename().string();
	}
	std::string FileHelper::GetParentDirectory(const std::string& path)
	{
		std::filesystem::path _path(path);
		return _path.parent_path().string();
	}
	std::string FileHelper::GetFileExtension(const std::string& path)
	{
		if (IsFile(path))
		{
			return std::filesystem::path(path).extension().string();
		}

		throw FileException(__FILE__, __LINE__, path + " - is not a file path");
	}
}