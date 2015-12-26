#pragma once
#include "ISimpleVar.h"

class CVar : public ISimpleVar
{
public:
	CVar(const std::string &name, VarType type);
	
	VarKind GetVarKind() const override;
};

