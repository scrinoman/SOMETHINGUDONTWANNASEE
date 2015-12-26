#include "stdafx.h"
#include "RuleTable.h"

using namespace std;

CRuleTable::CRuleTable(const vector<CRuleRow> &rows, GrammarTypes type)
	:m_rows(rows), m_lastFound(-1), m_type(type)
{
	m_isRow = m_rows.size() == 1;
}

int CRuleTable::FindNextRule(TokenType type) const
{
	for (size_t i = 0; i < m_rows.size(); ++i)
	{
		if (m_rows[i].Contains(type))
		{
			m_lastFound = i;
			return m_rows[i].GetNextPointer();
		}
	}

	return -1;
}

CRuleRow CRuleTable::GetRow(size_t index) const
{
	return m_rows[index];
}

bool CRuleTable::NeedReturn() const
{
	return m_type != GrammarTypes::LEFT_NONTERMINAL;
}

GrammarTypes CRuleTable::GetType() const
{
	return m_type;
}
//bool CRuleTable::IsLast() const
//{
//	return m_isLast;
//}