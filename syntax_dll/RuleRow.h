#pragma once
#include "syntax_dll.h"
#include "TokenType.h"
#include <string>
#include <vector>

SYNTAX_DLL_API class CRuleRow
{
public:
	CRuleRow(std::string name, const std::vector<TokenType> &types, int pointer);
	bool Contains(TokenType type) const;
	int GetNextPointer() const;
	std::string GetRuleName() const;

private:
	std::string m_ruleName;
	std::vector<TokenType> m_types;
	int m_nextStep;
};
