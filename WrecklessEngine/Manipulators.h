#pragma once
#include "InputOutputStream.h"

namespace IO
{
	class EndLine : public IManipulator
	{
	public:
		virtual IOutput& Execute(IOutput& output) override;
		virtual ~EndLine() = default;
	} static endl;
}