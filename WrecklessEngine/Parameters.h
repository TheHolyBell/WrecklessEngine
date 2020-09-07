#pragma once

#include "IParameter.h"
#include "ScriptString.h"


namespace Scripting
{
	class Integer : public IParameter
	{
	public:
		Integer() = default;
		Integer(int value) noexcept;

		Integer(const Integer& rhs) = default;
		Integer& operator=(const Integer& rhs) = default;

		Integer(Integer&& rhs) = default;
		Integer& operator=(Integer&& rhs) = default;

		operator int() const noexcept;
		virtual void* ptr() noexcept override;
	private:
		int m_Value = 0;
	};

	class UInt : public IParameter
	{
	public:
		UInt() = default;
		UInt(unsigned value) noexcept;

		UInt(const UInt& rhs) = default;
		UInt& operator=(const UInt& rhs) = default;

		UInt(UInt&& rhs) = default;
		UInt& operator=(UInt&& rhs) = default;

		operator unsigned() const noexcept;
		virtual void* ptr() noexcept override;
	private:
		unsigned m_Value = 0;
	};

	class Float : public IParameter
	{
	public:
		Float() = default;
		Float(float value) noexcept;

		Float(const Float& rhs) = default;
		Float& operator=(const Float& rhs) = default;

		Float(Float&& rhs) = default;
		Float& operator=(Float&& rhs) = default;

		operator float() const noexcept;
		virtual void* ptr() noexcept override;
	private:
		float m_Value = 0;
	};

	class Double : public IParameter
	{
	public:
		Double() = default;
		Double(double value) noexcept;

		Double(const Double& rhs) = default;
		Double& operator=(const Double& rhs) = default;

		Double(Double&& rhs) = default;
		Double& operator=(Double&& rhs) = default;

		operator double() const noexcept;
		virtual void* ptr() noexcept override;
	private:
		double m_Value = 0;
	};
}