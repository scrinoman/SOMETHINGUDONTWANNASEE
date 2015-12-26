#include "stdafx.h"
#include "RuleRow.h"

using namespace std;

CRuleRow::CRuleRow(string name, const vector<TokenType> &types, int pointer, Labels start, Labels end)
	:m_ruleName(std::move(name)), m_types(types), m_nextStep(pointer), m_start(start), m_end(end)
{
}

bool CRuleRow::Contains(TokenType type) const
{
	for (size_t i = 0; i < m_types.size(); ++i)
	{
		if (m_types[i] == type)
		{
			return true;
		}
	}

	return false;
}

int CRuleRow::GetNextPointer() const
{
	return m_nextStep;
}

string CRuleRow::GetRuleName() const
{
	return m_ruleName;
}

Labels CRuleRow::GetStartLabel() const
{
	return m_start;
}

Labels CRuleRow::GetEndLabel() const
{
	return m_end;
}