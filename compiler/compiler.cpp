#include "stdafx.h"
#include "Lexer.h"
#include "syntax_dll.h"
#include "SyntaxTable.h"
#include "Semantics.h"

using namespace std;

const string COMPILER_ARGUMENTS_LINE = "compiler.exe - [sourceCode] - {exeFileName}";

bool CheckArgsCount(int argc)
{
	if (argc < 2)
	{
		cout << "Not enough arguments: " << COMPILER_ARGUMENTS_LINE << endl;
		return false;
	}
	else
	{
		if (argc > 3)
		{
			cout << "Too many arguments: " << COMPILER_ARGUMENTS_LINE << endl;
			return false;
		}
	}

	return true;
}

bool ProccessLexer(const string &fileName, TokenTable &table)
{
	cout << "Lexer works... ";

	auto res = ParseFile(fileName);

	if (res.error)
	{
		cout << endl << "Error ! " << res.errorMessage << endl;
	}
	else
	{
		table = std::move(res.table);
		cout << "OK!" << endl;
	}

	return !res.error;
}

bool ProccessSyntax(const TokenTable &lexTable)
{
	cout << "Syntax works... ";

	auto res = CreateSyntaxTable(lexTable);

	if (res.isError)
	{
		cout << endl << "Error ! " << res.line << " line : " << res.lexem << endl;
	}
	else
	{
		cout << "OK!" << endl;
	}

	return !res.isError;
}

void DebugLexer(const TokenTable &lexTable)
{
	ofstream fout("lexer_result.txt");
	for (size_t i = 0; i < lexTable.size(); ++i)
	{
		for (size_t j = 0; j < lexTable[i].tokens.size(); ++j)
		{
			fout << lexTable[i].row << " " << lexTable[i].tokens[j].tokenString << " " << lexTable[i].tokens[j].strType << endl;
		}
	}
}

int main(int argc, char* argv[])
{
	if (!CheckArgsCount(argc))
	{
		return 0;
	}

	string sourceFile = argv[1];
	TokenTable lexTable;
	if (ProccessLexer(sourceFile, lexTable))
	{
		#ifdef _DEBUG
			DebugLexer(lexTable);
		#endif

		if (ProccessSyntax(lexTable))
		{
			//semantics&compile
		}
	}

	return 0;
}

