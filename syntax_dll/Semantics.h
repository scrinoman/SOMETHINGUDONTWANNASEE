#pragma once

#include "Lexer.h"
#include "SemanticTypes.h"
#include <stack>

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
};