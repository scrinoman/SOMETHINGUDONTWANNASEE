#pragma once
#include "SemanticTypes.h"
#include <string>

class ISimpleVar
{
public:
	ISimpleVar(const std::string &name, VarType type);

	virtual VarKind GetVarKind() const = 0;

	std::string GetName() const;
	VarType GetVarType() const;

private:
	std::string m_name;
	VarType m_type;
};

