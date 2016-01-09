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
#include "Semantics.h"

using namespace std;

bool StartParseRightPart(const TokenTable &lexTable, size_t &lexPointer, size_t &tokenPointer);
bool StartParseCondExpr(const TokenTable &lexTable, size_t &lexPointer, size_t &tokenPointer);

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
	stack<Labels> stLabels;
	stack<int> stScopeGrammarLevel;
	stack<int> stLabelsScopeGrammarLevel;
	int scopeGrammarLevel = 0;

	while (lexPointer < lexTable.size() && tokenPointer < lexTable[lexPointer].tokens.size() && !CSemantics::FoundError())
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
			if (grammarTable[pointer].GetRow(0).GetStartLabel() != Labels::LABEL_NONE)
			{
				CSemantics::Push(CSemantics::StackType(grammarTable[pointer].GetRow(0).GetStartLabel()));
			}

			if (grammarTable[pointer].GetRow(0).GetEndLabel() != Labels::LABEL_NONE)
			{
				if (grammarTable[pointer].GetType() == GrammarTypes::NONTERMINAL ||
					grammarTable[pointer].GetType() == GrammarTypes::LAST_NONTERMINAL ||
					grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING_AND_LAST_NONTERMINAL)
				{
					stLabelsScopeGrammarLevel.push(scopeGrammarLevel);
					stLabels.push(grammarTable[pointer].GetRow(0).GetEndLabel());
				}
			}

			if (grammarTable[pointer].GetType() == GrammarTypes::NONTERMINAL)
			{
				stScopeGrammarLevel.push(scopeGrammarLevel);
				stPointers.push(pointer + 1);
			}

			if (grammarTable[pointer].GetType() == GrammarTypes::TERMINAL)
			{
				CSemantics::Push(tokens[tokenPointer]);
				if (grammarTable[pointer].GetRow(0).GetEndLabel() != Labels::LABEL_NONE)
				{
					CSemantics::Push(CSemantics::StackType(grammarTable[pointer].GetRow(0).GetEndLabel()));
				}

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

				if (grammarTable[pointer].GetRow(0).GetEndLabel() != Labels::LABEL_NONE)
				{
					CSemantics::Push(CSemantics::StackType(grammarTable[pointer].GetRow(0).GetEndLabel()));
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
				std::stack<Labels> curLabels;
				int topLevelScope = stScopeGrammarLevel.empty() ? 0 : stScopeGrammarLevel.top();
				int topLevelLabelScope = stLabelsScopeGrammarLevel.empty() ? -1 : stLabelsScopeGrammarLevel.top();
				while (topLevelScope <= topLevelLabelScope)
				{
					curLabels.push(stLabels.top());
					stLabels.pop();

					stLabelsScopeGrammarLevel.pop();
					topLevelLabelScope = stLabelsScopeGrammarLevel.empty() ? -1 : stLabelsScopeGrammarLevel.top();
				}

				while (!curLabels.empty() && !CSemantics::FoundError())
				{
					CSemantics::Push(curLabels.top());
					curLabels.pop();
				}

				if (stPointers.empty())
				{
					return true;
				}

				pointer = stPointers.top();
				stPointers.pop();

				scopeGrammarLevel = stScopeGrammarLevel.top();
				stScopeGrammarLevel.pop();
			}
			else
			{
				scopeGrammarLevel++;
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

	int scopeLevel = 0;
	while (!CSemantics::FoundError())
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

				if (scopeLevel <= 0 && type == ")")
				{
					type = "EOLN";
				}

				break;
			}
			auto action = slrRightPartTable[curState].GetAction(type);
			if (action.action == CSLRRow::Action::ActionType::REJECTED)
			{
				std::cout << "Unexpected symbol " << token.tokenString << std::endl;
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
						
						if (type == "(")
						{
							scopeLevel++;
						}
						else
						{
							if (type == ")")
							{
								scopeLevel--;
							}
						}

						if (type == "low_bin_op" || type == "high_bin_op" || type == "(" || type == ")" || type == "const")
						{
							CSemantics::Push(token);
						}

						if (type == "-") //unary minus
						{
							CSemantics::Push(Token("unary minus", TokenType::UNARY_MINUS));
						}

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
	//std::cout << "Expected more tokens in arithmetic expression" << std::endl;
	return false;
}

bool StartParseCondExpr(const TokenTable &lexTable, size_t &lexPointer, size_t &tokenPointer)
{
	stack<int> states;
	stack<string> gotoStates;
	int curState = 0;
	states.push(0);
	bool needGoto = false;
	const std::vector<string> gotoReduces = { "", "S", "S", "A", "B", "B", "B", "A" };
	const std::vector<int> stackPopOffsets = { 0, 3, 1, 3, 3, 4, 1, 1};
	int scopeLevel = 0;
	while (!CSemantics::FoundError())
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
			case TokenType::INTEGER_DEC_NUMBER:
			case TokenType::FLOAT_NUMBER:
			case TokenType::STRING:
			case TokenType::CHARACTER:
			case TokenType::MINUS:
				type = "right_part";
				break;
			case TokenType::LOGICAL_AND:
			case TokenType::LOGICAL_OR:
				type = "bool_op";
				break;
			case TokenType::LESS:
			case TokenType::GREATER:
			case TokenType::LESS_OR_EQUAL:
			case TokenType::GREATER_OR_EQUAL:
			case TokenType::EQUAL:
			case TokenType::NOT_EQUAL:
				type = "rel_op";
				break;
			default:
				type = token.tokenString;
				if (type != ")" && type != "(" && type != "!")
				{
					type = "EOLN";
				}

				if (scopeLevel <= 0 && type == ")")
				{
					type = "EOLN";
				}

				break;
			}
			auto action = slrCondExprTable[curState].GetAction(type);
			if (action.action == CSLRRow::Action::ActionType::REJECTED)
			{
				std::cout << "Incorrect symbol " << token.tokenString << std::endl;
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

						if (type == "(")
						{
							scopeLevel++;
						}
						else
						{
							if (type == ")")
							{
								scopeLevel--;
							}
						}

						if (type == "bool_op" || type == "rel_op" || type == "(" || type == ")" || type == "!")
						{
							CSemantics::Push(token);
						}

						if (type == "right_part")
						{
							CSemantics::Push(Labels::START_ARITHMETIC_OPERATION); //костыль
							if (!StartParseRightPart(lexTable, lexPointer, tokenPointer))
							{
								CSemantics::Push(Labels::END_ARITHMETIC_OPERATION);
								return false;
							}
							CSemantics::Push(Labels::END_ARITHMETIC_OPERATION);
						}
						else
						{
							DoShift(lexTable, lexPointer, tokenPointer);
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
			auto action = slrCondExprTable[curState].GetAction(gotoStates.top());
			gotoStates.pop();
			if (action.action != action.GOTO)
			{
				assert(false, "GOTO without actual goto");
			}
			curState = action.passTo;
			states.push(curState);
		}
	}

	//std::cout << "Expected more tokens in condition" << std::endl;
	return false;
}

SyntaxError CreateSyntaxTable(const TokenTable &lexTable)
{
	int pointer = 1;
	int curScopeLevel = 0;
	stack<int> stPointers;
	stack<Labels> stLabels;
	stack<int> stScopeGrammarLevel;
	stack<int> stLabelsScopeGrammarLevel;
	int scopeGrammarLevel = 0;
	size_t row = 0;

	size_t lexPointer = 0;
	size_t tokenPointer = 0;
	CSemantics::Push(CSemantics::StackType(Labels::START_CODE));
	while (lexPointer < lexTable.size() && tokenPointer < lexTable[lexPointer].tokens.size() && !CSemantics::FoundError())
	{
		row = lexTable[lexPointer].row;
		auto &tokens = lexTable[lexPointer].tokens;
		int foundRule = grammarTable[pointer].FindNextRule(tokens[tokenPointer].type);
		if (foundRule == -1)
		{
			return SyntaxError(true, tokens[tokenPointer].tokenString, row, SyntaxErrorType::UNEXPECTED_SYMBOL);
		}
		else
		{
			if (grammarTable[pointer].GetRow(0).GetStartLabel() != Labels::LABEL_NONE)
			{
				CSemantics::Push(CSemantics::StackType(grammarTable[pointer].GetRow(0).GetStartLabel()));
			}

			if (grammarTable[pointer].GetRow(0).GetEndLabel() != Labels::LABEL_NONE)
			{
				if (grammarTable[pointer].GetType() == GrammarTypes::NONTERMINAL ||
					grammarTable[pointer].GetType() == GrammarTypes::LAST_NONTERMINAL ||
					grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING_AND_LAST_NONTERMINAL)
				{
					stLabelsScopeGrammarLevel.push(scopeGrammarLevel);
					stLabels.push(grammarTable[pointer].GetRow(0).GetEndLabel());
				}
			}

			if (grammarTable[pointer].GetType() == GrammarTypes::NONTERMINAL)
			{
				stScopeGrammarLevel.push(scopeGrammarLevel);
				stPointers.push(pointer + 1);
			}

			if (grammarTable[pointer].GetType() == GrammarTypes::TERMINAL)
			{
				CSemantics::Push(tokens[tokenPointer]);
				if (grammarTable[pointer].GetRow(0).GetEndLabel() != Labels::LABEL_NONE)
				{
					CSemantics::Push(CSemantics::StackType(grammarTable[pointer].GetRow(0).GetEndLabel()));
				}

				DoShift(lexTable, lexPointer, tokenPointer);
			}

			if (grammarTable[pointer].GetType() == GrammarTypes::NEED_SLR_PARSING)
			{
				if (grammarTable[pointer].GetRow(0).GetRuleName() == "right_part")
				{
					if (!StartParseRightPart(lexTable, lexPointer, tokenPointer))
					{
						return SyntaxError(true, "Error while parsing arithmetic expression! ", row, SyntaxErrorType::UNEXPECTED_SYMBOL);
					}
				}
				else
				{
					if (!StartParseCondExpr(lexTable, lexPointer, tokenPointer))
					{
						return SyntaxError(true, "Error while parsing condition expression! ", row, SyntaxErrorType::UNEXPECTED_SYMBOL);
					}
				}

				if (grammarTable[pointer].GetRow(0).GetEndLabel() != Labels::LABEL_NONE)
				{
					CSemantics::Push(CSemantics::StackType(grammarTable[pointer].GetRow(0).GetEndLabel()));
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
							return SyntaxError(true, "Error while parsing arithmetic expression! ", row, SyntaxErrorType::UNEXPECTED_SYMBOL);
						}
					}
					else
					{
						if (!StartParseCondExpr(lexTable, lexPointer, tokenPointer))
						{
							return SyntaxError(true, "Error while parsing condition expression! ", row, SyntaxErrorType::UNEXPECTED_SYMBOL);
						}
					}
				}
			}

			if (foundRule == 0)
			{
				std::stack<Labels> curLabels;
				int topLevelScope = stScopeGrammarLevel.top();
				int topLevelLabelScope = stLabelsScopeGrammarLevel.empty() ? -1 : stLabelsScopeGrammarLevel.top();
				while (topLevelScope <= topLevelLabelScope)
				{
					curLabels.push(stLabels.top());
					stLabels.pop();

					stLabelsScopeGrammarLevel.pop();
					topLevelLabelScope = stLabelsScopeGrammarLevel.empty() ? -1 : stLabelsScopeGrammarLevel.top();
				}

				while (!curLabels.empty() && !CSemantics::FoundError())
				{
					CSemantics::Push(curLabels.top());
					curLabels.pop();
				}

				pointer = stPointers.top();
				stPointers.pop();

				scopeGrammarLevel = stScopeGrammarLevel.top();
				stScopeGrammarLevel.pop();
			}
			else
			{
				scopeGrammarLevel++;
				pointer = foundRule;
			}
		}
	}

	if (!stPointers.empty() || pointer != 3)
	{
		return SyntaxError(true, "Expected more tokens", row, SyntaxErrorType::EXPECTED_MORE_SYMBOLS);
	}

	CSemantics::Push(CSemantics::StackType(Labels::END_CODE));
	if (!CSemantics::LastCheck())
	{
		return SyntaxError(true, "Function \"main\" not found", row, SyntaxErrorType::EXPECTED_EOF);
	}

	return SyntaxError(false);
}