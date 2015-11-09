#include "stdafx.h"
#include "RuleRow.h"

using namespace std;

CRuleRow::CRuleRow(string name, const vector<TokenType> &types, int pointer)
	:m_ruleName(name), m_types(types), m_nextStep(pointer)
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