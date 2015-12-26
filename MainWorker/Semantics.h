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

	static void CreateFunction();
	static void CreateParamList();
	static void CreateParam();

	struct ComplexExpressionElement
	{
		bool isOperator;
		Operator op;
		VarType type;

		ComplexExpressionElement(Operator newOperator)
			: op(newOperator), type(VarType::TYPE_VOID), isOperator(true)
		{
		}

		ComplexExpressionElement(VarType type)
			: op(Operator::PLUS), type(type), isOperator(false)
		{
		}
	};

	static std::stack<ComplexExpressionElement> m_typeStack;

	static void CreateArithmeticExpression();
	static void CreateConditionExpression();
};