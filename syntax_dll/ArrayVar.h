#pragma once
#include "ISimpleVar.h"

class CArrayVar : public ISimpleVar
{
public:
	CArrayVar(const std::string &name, VarType type, size_t dimCount, size_t firstDim, size_t secondDim);
	
	VarKind GetVarKind() const override;

	size_t GetDimensionCount() const;
	size_t GetFirstDimension() const;
	size_t GetSecondDimension() const;

private:
	size_t m_dimCount;
	size_t m_firstDim;
	size_t m_secondDim;
};

