#include "stdafx.h"
#include "MapVar.h"

CMapVar::CMapVar(const std::string &name, VarType keyType, VarType valType)
	:m_keyType(keyType)
	, ISimpleVar(name, valType)
{
}

VarKind CMapVar::GetVarKind() const
{
	return VarKind::MAP_VAR;
}

VarType CMapVar::GetKeyType() const
{
	return m_keyType;
}