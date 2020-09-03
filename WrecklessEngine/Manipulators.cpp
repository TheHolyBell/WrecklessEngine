#include "Manipulators.h"

namespace IO
{
    IOutput& EndLine::Execute(IOutput& output)
    {
        output.Put('\n');
        return output;
    }
}