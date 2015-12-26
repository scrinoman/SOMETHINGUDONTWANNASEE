#pragma once
#include "ISimpleVar.h"

class CMapVar : public ISimpleVar
{
public:
	CMapVar(const std::string &name, VarType keyType, VarType valType);

	VarKind GetVarKind() const override;
	VarType GetKeyType() const;

private:
	VarType m_keyType;
};

