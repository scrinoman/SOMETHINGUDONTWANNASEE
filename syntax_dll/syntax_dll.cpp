#include "stdafx.h"
#include "syntax_dll.h"
#include "RuleTable.h"
#include "Table.h"
#include <string>
#include "SyntaxTable.h"

using namespace std;

SyntaxResult CreateSyntaxTable(const TokenTable &lexTable)
{
	CSyntaxTable *table = new CSyntaxTable("code", CSyntaxTable::NONTERMINAL);
	CSyntaxTable *curNode = table;
	SyntaxError error(false);

	int pointer = 1;
	int curScopeLevel = 0;
	stack<int> stPointers;
	//stack<CSyntaxTable*> stTree;
	size_t row = 0;

	for (int lexPointer = 0; lexPointer < lexTable.size(); ++lexPointer)
	{
		row = lexTable[lexPointer].row;
		auto &tokens = lexTable[lexPointer].tokens;
		int tokenPointer = 0;
		while (tokenPointer < tokens.size())
		{
			int foundRule = grammarTable[pointer].FindNextRule(tokens[tokenPointer].type);
			if (foundRule == -1)
			{
				return SyntaxResult(table, SyntaxError(true, tokens[tokenPointer].tokenString, row, SyntaxErrorType::UNEXPECTED_SYMBOL));
			}
			else
			{
				if (grammarTable[pointer].GetType() == GrammarTypes::NONTERMINAL)// || grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING)
				{
					stPointers.push(pointer + 1);
					//stTree.push(curNode);
				}

				if (grammarTable[pointer].GetType() == GrammarTypes::TERMINAL)
				{
					tokenPointer++;
				}

				if (grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING)
				{
					pointer++;
				}
				else
				{
					if (grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING_AND_LAST_NONTERMINAL)
					{
						pointer = stPointers.top();
						stPointers.pop();
					}
				}

				if (foundRule == 0)
				{
					if (stPointers.empty())
					{
						//����� ������ �������� ?
						return SyntaxResult(table, SyntaxError(true, "Stack error ", row, SyntaxErrorType::STACK_ERROR));
					}

					pointer = stPointers.top();
					stPointers.pop();

					//CSyntaxTable *newNode = new CSyntaxTable(grammarTable[pointer].GetRow(0).GetRuleName(), CSyntaxTable::SYNTAX_ELEM_TYPE::NONE);
					//curNode->AddChild(newNode);

					//curNode = stTree.top();
					//stTree.pop();
				}
				else
				{
					//if (grammarTable[pointer].GetType() != GrammarTypes::LEFT_NONTERMINAL)
					//{
					//	CSyntaxTable *newNode = new CSyntaxTable(grammarTable[foundRule].GetRow(0).GetRuleName(), CSyntaxTable::SYNTAX_ELEM_TYPE::NONE);
					//	curNode->AddChild(newNode);
					//	curNode = newNode;
					//	/*CSyntaxTable *newNode = new CSyntaxTable(grammarTable[pointer].GetRow(0).GetRuleName(), CSyntaxTable::NONTERMINAL);
					//	curNode->AddChild(newNode);
					//	curNode = newNode;*/
					//}
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