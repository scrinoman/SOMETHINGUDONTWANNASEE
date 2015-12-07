#include "stdafx.h"
#include "Param.h"

CParam::CParam(const std::string &name, VarType type)
	:m_name(name)
	, m_type(type)
{
}

std::string CParam::GetName() const
{
	return m_name;
}

VarType CParam::GetType() const
{
	return m_type;
}