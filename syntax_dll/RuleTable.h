#pragma once
#include "syntax_dll.h"
#include "RuleRow.h"
#include <vector>
#include <string>

SYNTAX_DLL_API class CRuleTable
{
public:
	CRuleTable(const std::vector<CRuleRow> &rows);
	int FindNextRule(TokenType type) const;
	CRuleRow GetRow(size_t index) const;
	int GetStackRow() const;
	
private:
	std::vector<CRuleRow> m_rows;
	bool m_isRow;
	mutable int m_lastFound;
};

