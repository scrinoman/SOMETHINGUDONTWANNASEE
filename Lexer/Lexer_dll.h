#pragma once

#ifdef LEXER_EXPORTS
#define LEX_DLL_API __declspec(dllexport)
#else
#define LEX_DLL_API __declspec(dllimport)
#endif
