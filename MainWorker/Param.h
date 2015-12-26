#pragma once
#include "SemanticTypes.h"
#include <string>

class CParam
{
public:
	CParam(const std::string &name, VarType type);
	
	std::string GetName() const;
	VarType GetType() const;

private:
	std::string m_name;
	VarType m_type;
};

