#pragma once
#include "TokenType.h"
#include "Lexer.h"
#include <vector>
#include <string>
#include <stdexcept>

#ifdef SYNTAX_DLL_EXPORTS
#define SYNTAX_DLL_API __declspec(dllexport)
#else
#define SYNTAX_DLL_API __declspec(dllimport)
#endif

enum SYNTAX_DLL_API SyntaxErrorType
{
	NONE,
	UNEXPECTED_SYMBOL,
	EXPECTED_MORE_SYMBOLS,
	STACK_ERROR,
	EXPECTED_EOF
};

struct SYNTAX_DLL_API SyntaxError
{
	bool isError;
	std::string lexem;
	size_t line;
	SyntaxErrorType type;

	SyntaxError(bool error, const std::string &errLexem = "", size_t errRow = 0, SyntaxErrorType errType = SyntaxErrorType::NONE)
		:isError(error), lexem(errLexem), line(errRow), type(errType)
	{
		if (errType == NONE && error)
		{
			throw new std::logic_error("SyntaxError - got error but without description");
		}
	}
};

struct SYNTAX_DLL_API SyntaxTable
{

};

struct SYNTAX_DLL_API SyntaxResult
{
	SyntaxTable table;
	SyntaxError error;

	SyntaxResult(SyntaxTable &syntaxTable, SyntaxError &syntaxError)
		:table(std::move(syntaxTable)), error(std::move(syntaxError))
	{
	}
};

SYNTAX_DLL_API SyntaxResult CreateSyntaxTable(const TokenTable &lexTable);