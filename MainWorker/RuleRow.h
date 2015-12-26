#pragma once
#include "syntax_dll.h"
#include "TokenType.h"
#include "SemanticTypes.h"
#include <string>
#include <vector>

SYNTAX_DLL_API class CRuleRow
{
public:
	CRuleRow(std::string name, const std::vector<TokenType> &types, int pointer, Labels start = Labels::LABEL_NONE, Labels end = Labels::LABEL_NONE);
	bool Contains(TokenType type) const;
	int GetNextPointer() const;
	std::string GetRuleName() const;
	Labels GetStartLabel() const;
	Labels GetEndLabel() const;

private:
	std::string m_ruleName;
	std::vector<TokenType> m_types;
	int m_nextStep;
	Labels m_start, m_end;
};
