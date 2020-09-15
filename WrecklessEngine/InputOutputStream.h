#pragma once
#include <string>
#include "CommonInclude.h"

namespace IO
{
	class IOutput;

	class IManipulator
	{
	public:
		virtual IOutput& Execute(IOutput& output) PURE;
		virtual ~IManipulator() = default;
	};

	class IOutput
	{
	public:
		IOutput() = default;

		IOutput(const IOutput& rhs) = delete;
		IOutput& operator=(const IOutput& rhs) = delete;

		IOutput(IOutput&& rhs) = delete;
		IOutput& operator=(IOutput&& rhs) = delete;

		virtual ~IOutput() = default;

		virtual void Put(char c) PURE;
		virtual IOutput& operator<<(char character) PURE;

		virtual IOutput& operator<<(int number) PURE;
		virtual IOutput& operator<<(short number) PURE;
		virtual IOutput& operator<<(long number) PURE;
		virtual IOutput& operator<<(long long number) PURE;

		virtual IOutput& operator<<(unsigned short number) PURE;
		virtual IOutput& operator<<(unsigned number) PURE;
		virtual IOutput& operator<<(unsigned long number) PURE;
		virtual IOutput& operator<<(unsigned long long number) PURE;

		virtual IOutput& operator<<(float number) PURE;
		virtual IOutput& operator<<(double number) PURE;

		virtual IOutput& operator<<(const char* str) PURE;
		virtual IOutput& operator<<(const std::string& str) PURE;
		virtual IOutput& operator<<(IManipulator& manip)  PURE;
	};

	class IInput
	{
	public:
		IInput() = default;

		IInput(const IInput& rhs) = delete;
		IInput& operator=(const IInput& rhs) = delete;

		IInput(IInput&& rhs) = delete;
		IInput& operator=(IInput&& rhs) = delete;

		virtual ~IInput() = default;
		
		virtual char Get() PURE;

		virtual IInput& operator>>(char& character) PURE;
				
		virtual IInput& operator>>(short& number) PURE;
		virtual IInput& operator>>(int& number) PURE;
		virtual IInput& operator>>(long & number) PURE;
		virtual IInput& operator>>(long long& number) PURE;
				
		virtual IInput& operator>>(unsigned& number) PURE;
		virtual IInput& operator>>(unsigned short& number) PURE;
		virtual IInput& operator>>(unsigned long& number) PURE;
		virtual IInput& operator>>(unsigned long long& number) PURE;
				
		virtual IInput& operator>>(float& number) PURE;
		virtual IInput& operator>>(double& number) PURE;
				
		virtual IInput& operator>>(char* str) PURE;
		virtual IInput& operator>>(std::string& str) PURE;
	};

	class IOStream
	{
	public:
		static IOutput& GetOutputStream();
		static IInput& GetInputStream();
	};

	static IOutput& cout = IOStream::GetOutputStream();
	static IInput& cin = IOStream::GetInputStream();
}