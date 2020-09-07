#include "Parameters.h"

namespace Scripting
{
	Integer::Integer(int value) noexcept
		: m_Value(value)
	{
	}
	Integer::operator int() const noexcept
	{
		return m_Value;
	}
	void* Integer::ptr() noexcept
	{
		return &m_Value;
	}
	Float::Float(float value) noexcept
		: m_Value(value)
	{
	}
	Float::operator float() const noexcept
	{
		return m_Value;
	}
	void* Float::ptr() noexcept
	{
		return &m_Value;
	}
	Double::Double(double value) noexcept
		: m_Value(value)
	{
	}
	Double::operator double() const noexcept
	{
		return m_Value;
	}
	void* Double::ptr() noexcept
	{
		return &m_Value;
	}
	UInt::UInt(unsigned value) noexcept
		: m_Value(value)
	{
	}
	UInt::operator unsigned() const noexcept
	{
		return m_Value;
	}
	void* UInt::ptr() noexcept
	{
		return &m_Value;
	}
}