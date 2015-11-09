#pragma once
#include "syntax_dll.h"
#include "RuleRow.h"
#include <vector>
#include <string>

enum GrammarTypes
{
	LEFT_NONTERMINAL,
	NONTERMINAL,
	TERMINAL,
	LAST_NONTERMINAL,
	EMPTY
};

SYNTAX_DLL_API class CRuleTable
{
public:
	CRuleTable(const std::vector<CRuleRow> &rows, GrammarTypes type = GrammarTypes::NONTERMINAL);
	int FindNextRule(TokenType type) const;
	CRuleRow GetRow(size_t index) const;
	bool NeedReturn() const;
	GrammarTypes GetType() const;
	//bool IsLast() const;

private:
	std::vector<CRuleRow> m_rows;
	bool m_isRow;
	mutable int m_lastFound;
	GrammarTypes m_type;
	/*bool m_isLeftTable;
	bool m_isLast;*/
};

