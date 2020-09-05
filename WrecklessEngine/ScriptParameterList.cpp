#include "ScriptParameterList.h"
#include "Parameters.h"

namespace Scripting
{
    ParameterList& ParameterList::Append(int value)
    {
        m_Parameters.emplace_back(std::make_shared<Integer>(value));
        return *this;
    }
    ParameterList& ParameterList::Append(float value)
    {
        m_Parameters.emplace_back(std::make_shared<Float>(value));
        return *this;
    }
    ParameterList& ParameterList::Append(double value)
    {
        m_Parameters.emplace_back(std::make_shared<Double>(value));
        return *this;
    }
    ParameterList& ParameterList::Append(const char* value)
    {
        m_Parameters.emplace_back(std::make_shared<String>(value));
        return *this;
    }
    void ParameterList::Reserve(int size)
    {
       m_Parameters.reserve(size);
    }
    int ParameterList::Size() const
    {
        return m_Parameters.size();
    }
    void** ParameterList::GetArgs()
    {
       m_Raw.reserve(m_Parameters.size());

       for (const auto& p : m_Parameters)
            m_Raw.push_back(p->ptr());
        return m_Raw.data();
    }
    ParameterList::~ParameterList()
    {
    }
}