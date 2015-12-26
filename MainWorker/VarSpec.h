#pragma once
#include "ISimpleVar.h"

class CVarSpec
{
public:
	CVarSpec(ISimpleVar *var, size_t depth);
	~CVarSpec();

	ISimpleVar* GetVar() const;
	size_t GetDepth() const;

private:
	ISimpleVar *m_var;
	size_t m_depth;
};

