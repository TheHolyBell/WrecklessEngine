#pragma once
#include <exception>
#include <string>

namespace Exceptions
{
	class WrecklessException : public std::exception
	{
	public:
		WrecklessException(const char* fileName, int line) noexcept;
		virtual const char* what() const override;
		virtual const char* GetType() const noexcept; // @ Used in some cases
		const std::string& GetFileName() const noexcept;
		int GetLine() const noexcept;
		std::string GetOriginalString() const noexcept;

		virtual ~WrecklessException() = default;
	private:
		std::string m_File;
		int m_Line;
	protected:
		mutable std::string m_WhatBuffer;
	};
}