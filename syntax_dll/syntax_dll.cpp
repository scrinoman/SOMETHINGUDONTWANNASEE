#include "stdafx.h"
#include "syntax_dll.h"
#include "RuleTable.h"
#include "Table.h"
#include <string>
#include "SyntaxTable.h"
#include <map>
#include <vector>
#include "ArithmeticTable.h"
#include "SLRRow.h"

using namespace std;

bool StartParseRightPart(const TokenTable &lexTable, size_t &lexPointer, size_t &tokenPointer);

void DoShift(const TokenTable &lexTable, size_t &lexPointer, size_t &tokenPointer)
{
	tokenPointer++;
	if (tokenPointer >= lexTable[lexPointer].tokens.size())
	{
		tokenPointer = 0;
		lexPointer++;
	}
}

bool StartParseRule(const TokenTable &lexTable, size_t &lexPointer, size_t &tokenPointer, size_t pointer)
{
	stack<int> stPointers;
	while (lexPointer < lexTable.size() && tokenPointer < lexTable[lexPointer].tokens.size())
	{
		size_t row = lexTable[lexPointer].row;
		auto &tokens = lexTable[lexPointer].tokens;

		int foundRule = grammarTable[pointer].FindNextRule(tokens[tokenPointer].type);
		if (foundRule == -1)
		{
			return false;
		}
		else
		{
			if (grammarTable[pointer].GetType() == GrammarTypes::NONTERMINAL)
			{
				stPointers.push(pointer + 1);
			}

			if (grammarTable[pointer].GetType() == GrammarTypes::TERMINAL)
			{
				DoShift(lexTable, lexPointer, tokenPointer);
			}

			if (grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING)
			{
				if (grammarTable[pointer].GetRow(0).GetRuleName() == "right_part")
				{
					if (!StartParseRightPart(lexTable, lexPointer, tokenPointer))
					{
						return false;
					}
				}
				else
				{

				}
				pointer++;
				continue;
			}
			else
			{
				if (grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING_AND_LAST_NONTERMINAL)
				{
					if (grammarTable[pointer].GetRow(0).GetRuleName() == "right_part")
					{
						if (!StartParseRightPart(lexTable, lexPointer, tokenPointer))
						{
							return false;
						}
					}
				}
			}

			if (foundRule == 0)
			{
				if (stPointers.empty())
				{
					return true; // в принципе костыль, чисто для левой части - в общем случае тут false
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

	if (!stPointers.empty())
	{
		return false;
	}

	return true;
}

Token GetNextToken(const TokenTable &lexTable, size_t &lexPointer, size_t &tokenPointer, bool doShift = true)
{
	if (lexPointer >= lexTable.size())
	{
		return Token("EOLN", TokenType::ERROR);
	}

	if (doShift)
	{
		DoShift(lexTable, lexPointer, tokenPointer);
		if (lexPointer >= lexTable.size())
		{
			return Token("EOLN", TokenType::ERROR);
		}
	}
	
	return lexTable[lexPointer].tokens[tokenPointer];
}

bool StartParseRightPart(const TokenTable &lexTable, size_t &lexPointer, size_t &tokenPointer)
{
	stack<int> states;
	stack<string> gotoStates;
	int curState = 0;
	states.push(0);
	bool needGoto = false, wasOp = true;
	const std::vector<string> gotoReduces = {"", "S", "S", "A", "A", "B", "B", "B", "B", "B"};
	const std::vector<int> stackPopOffsets = {0, 3, 1, 3, 1, 3, 4, 1, 2, 1};
	while (1)
	{
		if (!needGoto)
		{
			Token &token = GetNextToken(lexTable, lexPointer, tokenPointer, false);
			string type = "";
			switch (token.type)
			{
			case TokenType::ERROR:
				type = "EOLN";
				break;
			case TokenType::IDENTIFIER:
				type = "left_part";
				break;
			case TokenType::PLUS:
			case TokenType::MODULE:
			case TokenType::OR:
				type = "low_bin_op";
				break;
			case TokenType::AMPERSAND:
			case TokenType::DIVIDE:
			case TokenType::STAR:
				type = "high_bin_op";
				break;
			case TokenType::INTEGER_DEC_NUMBER:
			case TokenType::FLOAT_NUMBER:
			case TokenType::STRING:
			case TokenType::CHARACTER:
				type = "const";
				break;
			case TokenType::MINUS:
				if (wasOp)
				{
					type = "-";
				}
				else
				{
					type = "low_bin_op";
				}
				break;
			default:
				type = token.tokenString;
				if (type != ")" && type != "(")
				{
					type = "EOLN";
				}
				break;
			}
			auto action = slrRightPartTable[curState].GetAction(type);
			if (action.action == CSLRRow::Action::ActionType::REJECTED)
			{
				return false;
			}
			else
			{
				if (action.action == CSLRRow::Action::ActionType::ACCEPTED)
				{
					return true;
				}
				else
				{
					if (action.action == CSLRRow::Action::ActionType::SHIFT)
					{
						curState = action.passTo;
						states.push(curState);
						if (type == "left_part")
						{
							if (!StartParseRule(lexTable, lexPointer, tokenPointer, 93))
							{
								return false;
							}
						}
						else
						{
							DoShift(lexTable, lexPointer, tokenPointer);
						}

						if (type == "low_bin_op" || type == "high_bin_op" || type == "(")
						{
							wasOp = true;
						}
						else
						{
							wasOp = false;
						}
					}
					else
					{
						if (action.action == CSLRRow::Action::ActionType::REDUCE)
						{
							for (int i = 0; i < stackPopOffsets[action.passTo]; ++i)
							{
								states.pop();
							}
							curState = states.top();
							gotoStates.push(gotoReduces[action.passTo]);
							needGoto = true;
						}
						else
						{
							assert(false, "Goto in normal states");
						}
					}
				}
			}
		}
		else
		{
			needGoto = false;
			auto action = slrRightPartTable[curState].GetAction(gotoStates.top());
			gotoStates.pop();
			if (action.action != action.GOTO)
			{
				assert(false, "GOTO without actual goto");
			}
			curState = action.passTo;
			states.push(curState);
		}
	}
	return false;
}

SyntaxResult CreateSyntaxTable(const TokenTable &lexTable)
{
	CSyntaxTable *table = new CSyntaxTable("code", CSyntaxTable::NONTERMINAL);
	CSyntaxTable *curNode = table;
	SyntaxError error(false);

	int pointer = 1;
	int curScopeLevel = 0;
	stack<int> stPointers;
	size_t row = 0;

	size_t lexPointer = 0;
	size_t tokenPointer = 0;
	while (lexPointer < lexTable.size() && tokenPointer < lexTable[lexPointer].tokens.size())
	{
		row = lexTable[lexPointer].row;
		auto &tokens = lexTable[lexPointer].tokens;
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
				DoShift(lexTable, lexPointer, tokenPointer);
			}

			if (grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING)
			{
				if (grammarTable[pointer].GetRow(0).GetRuleName() == "right_part")
				{
					if (!StartParseRightPart(lexTable, lexPointer, tokenPointer))
					{
						return SyntaxResult(table, SyntaxError(true, "False in parsing right_part ", row, SyntaxErrorType::UNEXPECTED_SYMBOL));
					}
				}
				else
				{

				}
				pointer++;
				continue;
			}
			else
			{
				if (grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING_AND_LAST_NONTERMINAL)
				{
					if (grammarTable[pointer].GetRow(0).GetRuleName() == "right_part")
					{
						if (!StartParseRightPart(lexTable, lexPointer, tokenPointer))
						{
							return SyntaxResult(table, SyntaxError(true, "False in parsing right_part ", row, SyntaxErrorType::UNEXPECTED_SYMBOL));
						}
					}
				}
			}

			if (foundRule == 0)
			{
				if (stPointers.empty())
				{
					//такое вообще возможно ? - рофл, возможно
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

	if (!stPointers.empty() || pointer != 3)
	{
		return SyntaxResult(table, SyntaxError(true, "Need more ", row, SyntaxErrorType::EXPECTED_MORE_SYMBOLS));
	}

	return SyntaxResult(table, SyntaxError(false));
}