#include "stdafx.h"
#include "syntax_dll.h"
#include "RuleTable.h"
#include <string>

#define CRT CRuleTable
#define CRR CRuleRow

using namespace std;

static const vector<CRuleTable> table = {
	CRT({ CRR("1", { TokenType::AMPERSAND, TokenType::BRACKET_L }, 0) }),
	CRT({ CRR("2", {0, 1}, 2) }),

};