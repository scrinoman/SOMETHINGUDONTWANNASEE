#include "stdafx.h"
#include "Function.h"

CFunction::CFunction(const std::string &name, VarType type, ParamTable* params)
	:m_name(name)
	, m_type(type)
	, m_params(params)
{
}

std::string CFunction::GetName() const
{
	return m_name;
}

VarType CFunction::GetType() const
{
	return m_type;
}

ParamTable* CFunction::GetParams() const
{
	return m_params;
}
