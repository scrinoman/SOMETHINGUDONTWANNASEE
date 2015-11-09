#include "stdafx.h"
#include "syntax_dll.h"
#include "RuleTable.h"
#include "Table.h"
#include <string>

using namespace std;

SyntaxResult CreateSyntaxTable(const TokenTable &lexTable)
{
	SyntaxTable table;
	SyntaxError error(false);

	int pointer = 1;
	stack<int> stPointers;
	size_t row = 0;

	for (int lexPointer = 0; lexPointer < lexTable.size(); ++lexPointer)
	{
		row = lexTable[lexPointer].row;
		auto &tokens = lexTable[lexPointer].tokens;
		int tokenPointer = 0;
		//for (int tokenPointer = 0; tokenPointer < tokens.size(); ++tokenPointer)
		while (tokenPointer < tokens.size())
		{
			int foundRule = grammarTable[pointer].FindNextRule(tokens[tokenPointer].type);
			if (foundRule == -1)
			{
				return SyntaxResult(table, SyntaxError(true, tokens[tokenPointer].tokenString, row, SyntaxErrorType::UNEXPECTED_SYMBOL));
			}
			else
			{
				if (grammarTable[pointer].GetType() == GrammarTypes::NONTERMINAL)
				{
					stPointers.push(pointer + 1);
				}

				if (grammarTable[pointer].GetType() == GrammarTypes::TERMINAL)
				{
					tokenPointer++;
					/*if (grammarTable[pointer].GetRow(0).GetRuleName().length() == 1 || foundRule == 0 
						|| grammarTable[pointer].GetRow(0).GetRuleName() == "id"
						|| grammarTable[pointer].GetRow(0).GetRuleName() == "const")
					{
						tokenPointer++;
					}*/
				}

				if (foundRule == 0)
				{
					if (stPointers.empty())
					{
						//такое вообще возможно ?
						return SyntaxResult(table, SyntaxError(true, "Stack error ", row, SyntaxErrorType::STACK_ERROR));
					}

					pointer = stPointers.top();
					stPointers.pop();
				}
				else
				{
					pointer = foundRule;
				}
			}
		}
	}

	if (!stPointers.empty() || pointer != 3)
	{
		return SyntaxResult(table, SyntaxError(true, "Need more ", row, SyntaxErrorType::EXPECTED_MORE_SYMBOLS));
	}

	return SyntaxResult(table, SyntaxError(false));
}