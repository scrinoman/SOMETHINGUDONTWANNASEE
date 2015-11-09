#pragma once
#include "stdafx.h"
#include "lex_dll.h"
#include "TokenType.h"

std::map<std::string, TokenType> reservedTokens = {
	{ "char", TokenType::CHAR },
	{ "cin", TokenType::CIN },
	{ "cout", TokenType::COUT },
	{ "map", TokenType::MAP },
	{ "string", TokenType::STR },
	{ "else", TokenType::ELSE },
	{ "float", TokenType::FLOAT },
	{ "for", TokenType::FOR },
	{ "if", TokenType::IF },
	{ "int", TokenType::INT },
	{ "return", TokenType::RETURN },
	{ "void", TokenType::VOID },
	{ "while", TokenType::WHILE },
	{ ";", TokenType::SEMICOLON },
	{ ",", TokenType::COMMA },
	{ "{", TokenType::CURLY_BRACKET_L },
	{ "}", TokenType::CURLY_BRACKET_R },
	{ "(", TokenType::BRACKET_L },
	{ ")", TokenType::BRACKET_R },
	{ "[", TokenType::SQUARE_BRACKET_L },
	{ "]", TokenType::SQUARE_BRACKET_R },
	{ "~", TokenType::TILDE },
	{ "+", TokenType::PLUS },
	{ "-", TokenType::MINUS },
	{ "*", TokenType::STAR },
	{ "/", TokenType::DIVIDE },
	{ "%", TokenType::MODULE },
	{ ".", TokenType::DOT },
	{ "<", TokenType::LESS },
	{ ">", TokenType::GREATER },
	{ "<=", TokenType::LESS_OR_EQUAL },
	{ ">=", TokenType::GREATER_OR_EQUAL },
	{ "==", TokenType::EQUAL },
	{ "=", TokenType::ASSIGN },
	{ "&&", TokenType::LOGICAL_AND },
	{ "||", TokenType::LOGICAL_OR },
	{ "&", TokenType::AMPERSAND },
	{ "|", TokenType::OR },
	{ "^", TokenType::XOR },
	{ "!", TokenType::NOT },
	{ "!=", TokenType::NOT_EQUAL },
	{ "<<", TokenType::LEFT_SHIFT },
	{ ">>", TokenType::RIGHT_SHIFT },
};

std::set<std::string> delimiters = { ";", ",", "{", "}", "(", ")", "[", "]", "~", "+", "-", ".", "*", "/", "%", "<", ">", "<=", ">=",
		"==", "|", "=", "&&", "||", "&", "^", "!", "!=", "<<", ">>"};