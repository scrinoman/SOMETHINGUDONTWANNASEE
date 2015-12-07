#include "stdafx.h"
#include "VarSpec.h"

CVarSpec::CVarSpec(ISimpleVar *var, size_t depth)
	:m_var(var)
	, m_depth(depth)
{
}

CVarSpec::~CVarSpec()
{
	delete m_var;
}

ISimpleVar* CVarSpec::GetVar() const
{
	return m_var;
}

size_t CVarSpec::GetDepth() const
{
	return m_depth;
}