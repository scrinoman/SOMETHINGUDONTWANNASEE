#pragma once
#include "TokenType.h"
#include "Lexer.h"
#include <vector>
#include <string>

#ifdef SYNTAX_DLL_EXPORTS
#define SYNTAX_DLL_API __declspec(dllexport)
#else
#define SYNTAX_DLL_API __declspec(dllimport)
#endif



//typedef SYNTAX_DLL_API std::vector<SyntaxTableRow> SyntaxTable;
//struct SYNTAX_DLL_API SyntaxReport
//{
//	SyntaxTable table;
//
//};
//
//SYNTAX_DLL_API SyntaxReport CreateSyntaxTable(const TokenTable&);