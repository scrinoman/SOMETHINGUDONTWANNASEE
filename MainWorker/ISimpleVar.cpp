#include "stdafx.h"
#include "ISimpleVar.h"

ISimpleVar::ISimpleVar(const std::string &name, VarType type)
	:m_name(name)
	,m_type(type)
{
}

std::string ISimpleVar::GetName() const
{
	return m_name;
}

VarType ISimpleVar::GetVarType() const
{
	return m_type;
}