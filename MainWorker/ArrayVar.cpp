#include "stdafx.h"
#include "ArrayVar.h"

CArrayVar::CArrayVar(const std::string &name, VarType type, size_t dimCount, size_t firstDim, size_t secondDim)
	:m_dimCount(dimCount)
	, m_firstDim(firstDim)
	, m_secondDim(secondDim)
	, ISimpleVar(name, type)
{
}

VarKind CArrayVar::GetVarKind() const
{
	return VarKind::ARRAY;
}

size_t CArrayVar::GetDimensionCount() const
{
	return m_dimCount;
}

size_t CArrayVar::GetFirstDimension() const
{
	return m_firstDim;
}

size_t CArrayVar::GetSecondDimension() const
{
	return m_secondDim;
}