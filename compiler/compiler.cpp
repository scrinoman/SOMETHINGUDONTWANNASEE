#include "stdafx.h"
#include "Lexer.h"
#include "syntax_dll.h"
#include "SyntaxTable.h"
#include "Semantics.h"
#include <Windows.h>

const std::string COMPILER_ARGUMENTS_LINE = "compiler.exe - [sourceCode] - {exeFileName}";

bool CheckArgsCount(int argc)
{
	if (argc < 2)
	{
		std::cout << "Not enough arguments: " << COMPILER_ARGUMENTS_LINE << std::endl;
		return false;
	}
	else
	{
		if (argc > 3)
		{
			std::cout << "Too many arguments: " << COMPILER_ARGUMENTS_LINE << std::endl;
			return false;
		}
	}

	return true;
}

bool ProccessLexer(const std::string &fileName, TokenTable &table)
{
	std::cout << "Lexer works... ";

	auto res = ParseFile(fileName);

	if (res.error)
	{
		std::cout << std::endl << "Error ! " << res.errorMessage << std::endl;
	}
	else
	{
		table = std::move(res.table);
		std::cout << "OK!" << std::endl;
	}

	return !res.error;
}

bool ProccessSyntax(const TokenTable &lexTable)
{
	std::cout << "Syntax works... ";

	auto res = CreateSyntaxTable(lexTable);

	if (res.isError)
	{
		std::cout << std::endl << "Error ! " << res.line << " line : " << res.lexem << std::endl;
	}
	else
	{
		std::cout << "OK!" << std::endl;
	}

	return !res.isError;
}

void DebugLexer(const TokenTable &lexTable)
{
	std::ofstream fout("lexer_result.txt");
	for (size_t i = 0; i < lexTable.size(); ++i)
	{
		for (size_t j = 0; j < lexTable[i].tokens.size(); ++j)
		{
			fout << lexTable[i].row << " " << lexTable[i].tokens[j].tokenString << " " << lexTable[i].tokens[j].strType << std::endl;
		}
	}
}

bool RunCodeGenerator(const std::string &exeName)
{
	STARTUPINFO stInfo;
	PROCESS_INFORMATION procInfo;

	ZeroMemory(&stInfo, sizeof(stInfo));
	ZeroMemory(&procInfo, sizeof(procInfo));

	stInfo.cb = sizeof(stInfo);
	std::wstring ws(exeName.begin(), exeName.end());

	if (CreateProcess(L"CodeGenerator.exe", 
		L"CodeGenerator.exe wtf",//&ws[0],
		NULL, NULL, FALSE, 0, NULL, NULL,
		&stInfo, &procInfo))
	{
		WaitForSingleObject(procInfo.hProcess, INFINITE);
		return true;
	}
	else
	{
		return false;
	}
}

int main(int argc, char* argv[])
{
	if (!CheckArgsCount(argc))
	{
		return 0;
	}

	std::string sourceFile = argv[1];
	TokenTable lexTable;
	if (ProccessLexer(sourceFile, lexTable))
	{
		#ifdef _DEBUG
			DebugLexer(lexTable);
		#endif

		if (ProccessSyntax(lexTable))
		{
			CSemantics::CloseLogger();
			if (RunCodeGenerator(sourceFile))
			{
				std::cout << "Created !" << std::endl;
			}
			else
			{
				std::cout << "Error while launching code generator !" << std::endl;
			}
			
		}
	}

	return 0;
}

