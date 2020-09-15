#include <cstring>
#include "ConsoleAdapter.h"
#include "ConsoleClass.h"

namespace IO
{
	void ConsoleOutput::Put(char c)
	{

		(*this) << c;
	}

	IOutput& ConsoleOutput::operator<<(char character)
	{
		char _Str[2];
		_Str[0] = character;
		_Str[1] = '\0';
		Console::Write(_Str);
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(int number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(short number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(long number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(long long number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(unsigned short number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(unsigned number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(unsigned long number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(unsigned long long number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(float number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(double number)
	{
		Console::Write(std::to_string(number));
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(const char* str)
	{
		Console::Write(str);
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(const std::string& str)
	{
		Console::Write(str);
		return *this;
	}

	IOutput& ConsoleOutput::operator<<(IManipulator& manip)
	{
		return manip.Execute(*this);
	}

	


	char ConsoleInput::Get()
	{
		return Console::ReadKey();
	}

	IInput& ConsoleInput::operator>>(char& character)
	{
		character = Console::ReadKey();
		return *this;
	}

	IInput& ConsoleInput::operator>>(short& number)
	{
		number = atoi(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(int& number)
	{
		number = atoi(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(long& number)
	{
		number = atol(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(long long& number)
	{
		number = atoi(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(unsigned& number)
	{
		number = atoi(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(unsigned short& number)
	{
		number = atoi(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(unsigned long& number)
	{
		number = atol(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(unsigned long long& number)
	{
		number = atoll(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(float& number)
	{
		number = atof(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(double& number)
	{
		number = atof(Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(char* str)
	{
		strcpy(str, Console::ReadLine().c_str());
		return *this;
	}

	IInput& ConsoleInput::operator>>(std::string& str)
	{
		str = Console::ReadLine();
		return *this;
	}

}