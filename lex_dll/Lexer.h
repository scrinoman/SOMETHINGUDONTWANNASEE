#pragma once
#include "stdafx.h"
#include "lex_dll.h"
#include "TokenType.h"
#include "TokenTypeString.h"

struct LEX_DLL_API Token
{
	std::string tokenString;
	TokenType type;
	std::string strType;

	Token(std::string token, TokenType type)
		:tokenString(std::move(token)), type(type),
		strType(tokenStringName[type])
	{
	}
};

struct TokenGroup
{
	std::string tokenString;
	size_t row;

	TokenGroup(std::string &&token, size_t row)
		:tokenString(std::move(token)), row(row)
	{
	}
};

struct LEX_DLL_API TokenLine
{
	std::vector<Token> tokens;
	size_t row;

	TokenLine(std::vector<Token> &&tokens, size_t row)
		:tokens(std::move(tokens)), row(row)
	{
	}
};

typedef LEX_DLL_API std::vector<TokenLine> TokenTable;
LEX_DLL_API TokenTable ParseFile(const std::string &fNameInput);