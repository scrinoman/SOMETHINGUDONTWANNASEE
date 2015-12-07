#include "stdafx.h"
#include "Var.h"

CVar::CVar(const std::string &name, VarType type)
	:ISimpleVar(name, type)
{
}

VarKind CVar::GetVarKind() const
{
	return VarKind::VAR;
}