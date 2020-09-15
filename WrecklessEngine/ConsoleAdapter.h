#pragma once
#include "InputOutputStream.h"

namespace IO
{
	class ConsoleOutput : public IOutput
	{
	public:
		virtual void Put(char c) override;
		virtual IOutput& operator<<(char character);

		virtual IOutput& operator<<(int number) override;
		virtual IOutput& operator<<(short number) override;
		virtual IOutput& operator<<(long number) override;
		virtual IOutput& operator<<(long long number) override;

		virtual IOutput& operator<<(unsigned short number) override;
		virtual IOutput& operator<<(unsigned number) override;
		virtual IOutput& operator<<(unsigned long number) override;
		virtual IOutput& operator<<(unsigned long long number) override;

		virtual IOutput& operator<<(float number) override;
		virtual IOutput& operator<<(double number) override;

		virtual IOutput& operator<<(const char* str) override;
		virtual IOutput& operator<<(const std::string& str) override;
		virtual IOutput& operator<<(IManipulator& manip)  override;

		virtual ~ConsoleOutput() = default;
	};

	class ConsoleInput : public IInput
	{
	public:
		virtual char Get() override;

		virtual IInput& operator>>(char& character) override;
				
		virtual IInput& operator>>(short& number) override;
		virtual IInput& operator>>(int& number) override;
		virtual IInput& operator>>(long& number) override;
		virtual IInput& operator>>(long long& number) override;
				
		virtual IInput& operator>>(unsigned& number) override;
		virtual IInput& operator>>(unsigned short& number) override;
		virtual IInput& operator>>(unsigned long& number) override;
		virtual IInput& operator>>(unsigned long long& number) override;
				
		virtual IInput& operator>>(float& number) override;
		virtual IInput& operator>>(double& number) override;
				
		virtual IInput& operator>>(char* str) override;
		virtual IInput& operator>>(std::string& str) override;

		virtual ~ConsoleInput() = default;
	};
}