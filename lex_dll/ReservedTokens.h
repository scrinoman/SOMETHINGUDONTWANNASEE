#pragma once
#include "stdafx.h"
#include "lex_dll.h"
#include "TokenType.h"

std::map<std::string, TokenType> reservedTokens = {
	/*{ "__wchar_t", TokenType::WIDE_CHAR },
	{ "wchar_t", TokenType::WIDE_CHAR },
	{ "TCHAR", TokenType::T_CHAR },
	{ "_TCHAR", TokenType::T_CHAR },
	{ "auto", TokenType::AUTO },
	{ "break", TokenType::BREAK },
	{ "case", TokenType::CASE },*/
	{ "char", TokenType::CHAR },
	{ "cin", TokenType::CIN },
	{ "cout", TokenType::COUT },
	{ "map", TokenType::MAP },
	{ "string", TokenType::STR },
	/*{ "const", TokenType::CONST },
	{ "continue", TokenType::CONTINUE },
	{ "default", TokenType::DEFAULT },
	{ "decltype", TokenType::DECLTYPE },
	{ "do", TokenType::DO },
	{ "double", TokenType::DOUBLE },*/
	{ "else", TokenType::ELSE },
	//{ "enum", TokenType::ENUM },
	//{ "extern", TokenType::EXTERN },
	//{ "false", TokenType::FALSE },
	{ "float", TokenType::FLOAT },
	{ "for", TokenType::FOR },
	//{ "friend", TokenType::FRIEND },
	//{ "goto", TokenType::GOTO },
	{ "if", TokenType::IF },
	//{ "__int", TokenType::BITS_INT },
	{ "int", TokenType::INT },
	//{ "internal", TokenType::INTERNAL },
	//{ "long", TokenType::LONG },
	//{ "namespace", TokenType::NAMESPACE },
	//{ "operator", TokenType::OPERATOR },
	//{ "private", TokenType::PRIVATE },
	//{ "public", TokenType::PUBLIC },
	//{ "protected", TokenType::PROTECTED },
	//{ "register", TokenType::REGISTER },
	{ "return", TokenType::RETURN },
	/*{ "short", TokenType::SHORT },
	{ "signed", TokenType::SIGNED },
	{ "sizeof", TokenType::SIZEOF },
	{ "static", TokenType::STATIC },
	{ "struct", TokenType::STRUCT },
	{ "switch", TokenType::SWITCH },
	{ "typedef", TokenType::TYPEDEF },
	{ "throw", TokenType::THROW },
	{ "true", TokenType::TRUE },
	{ "try", TokenType::TRY },
	{ "union", TokenType::UNION },
	{ "unsigned", TokenType::UNSIGNED },
	{ "using", TokenType::USING },*/
	{ "void", TokenType::VOID },
	//{ "volatile", TokenType::VOLATILE },
	{ "while", TokenType::WHILE },
	{ ";", TokenType::SEMICOLON },
	{ ",", TokenType::COMMA },
	{ "{", TokenType::CURLY_BRACKET_L },
	{ "}", TokenType::CURLY_BRACKET_R },
	//{ ":", TokenType::COLON },
	{ "(", TokenType::BRACKET_L },
	{ ")", TokenType::BRACKET_R },
	{ "[", TokenType::SQUARE_BRACKET_L },
	{ "]", TokenType::SQUARE_BRACKET_R },
	{ "~", TokenType::TILDE },
	//{ "?", TokenType::TERNARY_IF },
	//{ "...", TokenType::ELLIPSIS },
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
	//{ "|", TokenType::PIPE },
	{ "=", TokenType::ASSIGN },
	{ "&&", TokenType::LOGICAL_AND },
	{ "||", TokenType::LOGICAL_OR },
	{ "&", TokenType::AMPERSAND },
	{ "|", TokenType::OR },
	{ "^", TokenType::XOR },
	//{ "!", TokenType::NOT },
	{ "!=", TokenType::NOT_EQUAL },
	/*{ "->", TokenType::PTR },
	{ "++", TokenType::INC },
	{ "--", TokenType::DEC },*/
	{ "<<", TokenType::LEFT_SHIFT },
	{ ">>", TokenType::RIGHT_SHIFT },
	/*{ "+=", TokenType::ADD_ASSIGN },
	{ "-=", TokenType::SUB_ASSIGN },
	{ "*=", TokenType::MUL_ASSIGN },
	{ "/=", TokenType::DIV_ASSIGN},
	{ "%=", TokenType::MOD_ASSIGN },
	{ "&=", TokenType::AND_ASSIGN },
	{ "|=", TokenType::OR_ASSIGN },
	{ "^=", TokenType::XOR_ASSIGN },
	{ "<<=", TokenType::LEFT_SHIFT_ASSIGN },
	{ ">>=", TokenType::RIGHT_SHIFT_ASSIGN }*/
};

std::set<std::string> delimiters = { ";", ",", "{", "}", /*":",*/ "(", ")", "[", "]", /*"~","?", "...",*/ "+", "-", ".", "*", "/", "%", "<", ">", "<=", ">=",
		"==", "|", "=", "&&", "||", "&", "^", "!", "!=", /*"->", "++", "--",*/ "<<", ">>"/*, "+=", "-=", "*=", "/=", "%=", "&=",	"|=", "^=",*/
		/*"<<=", ">>="*/ };