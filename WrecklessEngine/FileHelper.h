#pragma once

#include <string>
#include "WrecklessException.h"

namespace FileSystem
{
	class FileHelper
	{
	public:
		class FileException : public Exceptions::WrecklessException
		{
		public:
			FileException(const char* file, int line, const std::string& msg);
			virtual const char* what() const override;
			virtual const char* GetType() const noexcept override;
		private:
			std::string m_Message;
		};

		static bool Exists(const std::string& path);
		static void CreateFolder(const std::string& path);
		static void RemoveFile(const std::string& path);

		static std::string GetCurrentWorkingDirectory();

		static bool IsFile(const std::string& path);
		static bool IsDirectory(const std::string& path);

		static std::string GetDirectoryFromPath(const std::string& path);
		static std::string GetFileNameFromPath(const std::string& path);
		static std::string GetParentDirectory(const std::string& path);
		static std::string GetFileExtension(const std::string& path);
	};
}