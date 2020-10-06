#include "InputOutputStream.h"
#include "ConsoleAdapter.h"
#include "ImGuiLogger.h"

namespace IO
{

    IOutput& IOStream::GetOutputStream()
    {
        static ConsoleOutput _Output;
        return _Output;
    }

    IInput& IOStream::GetInputStream()
    {
        static ConsoleInput _Input;
        return _Input;
    }
}