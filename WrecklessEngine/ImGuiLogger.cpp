#include "ImGuiLogger.h"
#include "ImGui/imgui.h"

namespace IO
{
	std::vector<std::string> ImGuiOutput::m_Messages;
	std::ostringstream ImGuiOutput::m_Oss;

	void ImGuiOutput::Draw()
	{
		if (ImGui::Begin("ImGui Output Console"))
		{
			for (const auto& msg : m_Messages)
				ImGui::Text(msg.c_str());
		}
		ImGui::End();
	}
	void ImGuiOutput::Put(char c)
	{
		m_Oss << c;
	}
	IOutput& ImGuiOutput::operator<<(char character)
	{
		m_Oss << character;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(int number)
	{
		m_Oss << number;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(short number)
	{
		m_Oss << number;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(long long number)
	{
		m_Oss << number;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(unsigned short number)
	{
		m_Oss << number;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(unsigned number)
	{
		m_Oss << number;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(unsigned long long number)
	{
		m_Oss << number;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(float number)
	{
		m_Oss << number;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(double number)
	{
		m_Oss << number;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(const char* str)
	{
		m_Oss << str;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(const std::string& str)
	{
		m_Oss << str;
		return *this;
	}
	IOutput& ImGuiOutput::operator<<(IManipulator& manip)
	{
		manip.Execute(*this);
		m_Messages.emplace_back(m_Oss.str());
		m_Oss.clear();
		return *this;
	}
}