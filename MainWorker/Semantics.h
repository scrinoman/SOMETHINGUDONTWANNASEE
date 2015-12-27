#pragma once

#include "Lexer.h"
#include "SemanticTypes.h"
#include <stack>
#include <set>
#include "TokenType.h"

class CSemantics
{
public:
	struct StackType
	{
		Token token;
		Labels label;

		bool isToken;

		StackType(const Token &newToken)
			:token(newToken), label(Labels::LABEL_NONE), isToken(true)
		{
		}

		StackType(const Labels newLabel)
			:token("", TokenType::ERROR), label(newLabel), isToken(false)
		{
		}
	};
	
	static void Push(StackType &&element);
	static StackType Pop();
	static void LogToFile();

private:
	static std::stack<CSemantics::StackType> m_stack;
	static std::stack<void*> m_elems;

	const static std::set<TokenType> m_forbiddenSymbols;
	const static std::map < Operator, int > m_operationPrior;
	static int curScope;

	static void CreateFunction();
	static void CreateParamList();
	static void CreateParam();
	static void AddVarToTable();
	static void RecognizeLeftPart();
	static void RecognizeArrayPart();
	static void RecognizeFuncCall();
	static void RecognizeSimpleVar();
	static int GetVarInTable(const std::string &varName);

	static std::stack<ComplexExpression> m_evalStack;
	static std::stack<VariableDescription*> m_varStack;
	static std::stack<VarType> m_types;
	static std::vector<VarElement> m_varTable;

	static void CreateArithmeticExpression();
	static void CreateConditionExpression();

	static Operator GetOperatorByString(const std::string &opString);
};