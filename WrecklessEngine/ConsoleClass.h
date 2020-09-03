#pragma once
#include <Windows.h>
#include <string>

enum class ConsoleColor
{
    Black = 0,
    DarkBlue = 1,
    DarkGreen = 2,
    DarkCyan = 3,
    DarkRed = 4,
    DarkMagenta = 5,
    DarkYellow = 6,
    Gray = 7,
    DarkGray = 8,
    Blue = 9,
    Green = 10,
    Cyan = 11,
    Red = 12,
    Magenta = 13,
    Yellow = 14,
    White = 15
};

class Console
{
public:
	static void Initialize();

	static bool WriteLine(const char* fmt, ...);
	static bool Write(const char* fmt, ...);
	

	static bool WriteLine(const std::string& fmt);
	static bool Write(const std::string&  fmt);

    static std::string ReadLine();
	
    static char ReadKey();
    static  int Read();

    static void SetConsoleColor(ConsoleColor color);

	static void Shutdown();
private:
	static void AttachConsole();
	static void DetachConsole();

private:
	static HANDLE _out, _old_out;
	static HANDLE _err, _old_err;
	static HANDLE _in, _old_in;
};