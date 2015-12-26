#pragma once
#include "VarSpec.h"
#include "Param.h"
#include <vector>

template <class elemType>
class SemanticTable
{
public:
	SemanticTable()
		:m_elems()
	{
	}

	size_t GetCount() const
	{
		return m_elems.size();
	}

	elemType GetElement(size_t index) const
	{
		return m_elems[index];
	}
	
	void AddElement(elemType MOVE_PARAM_DECL &&elem)
	{
		m_elems.push_back(std::forward<elemType>(elem));
	}

private:
	std::vector<elemType> m_elems;
};

class ParamTable : public SemanticTable<CParam*>
{
};

class VarTable : public SemanticTable<CVarSpec*>
{
};

class ActionTable
{
};