#pragma once
#include "stdafx.h"
#include "TokenType.h"

struct Token
{
	std::string tokenString;
	TokenType type;

	Token(std::string token, TokenType type)
		:tokenString(std::move(token)), type(type)
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

struct TokenLine
{
	std::vector<Token> tokens;
	size_t row;

	TokenLine(std::vector<Token> &&tokens, size_t row)
		:tokens(std::move(tokens)), row(row)
	{
	}
};

typedef std::vector<TokenLine> TokenTable;
TokenTable ParseFile(const std::string &fNameInput);