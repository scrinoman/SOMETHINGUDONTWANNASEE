// lex_dll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "lex_dll.h"


// Пример экспортированной переменной
LEX_DLL_API int nlex_dll=0;

// Пример экспортированной функции.
LEX_DLL_API int fnlex_dll(void)
{
	return 42;
}

// Конструктор для экспортированного класса.
// см. определение класса в lex_dll.h
Clex_dll::Clex_dll()
{
	return;
}
