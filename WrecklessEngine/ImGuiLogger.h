#pragma once

#include "CommonInclude.h"
#include "InputOutputStream.h"
#include <sstream>

namespace IO
{
	class ImGuiOutput : public IOutput
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
		static void Clear();

		virtual ~ImGuiOutput() = default;

		static void Draw(bool app_is_running = false);
	private:
		static std::vector<std::string> m_Messages;
		static std::ostringstream m_Oss;
	};
}