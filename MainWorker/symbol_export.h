#pragma once

#ifdef MAINWORKER_EXPORTS
#define SYNTAX_DLL_API __declspec(dllexport)
#else
#define SYNTAX_DLL_API __declspec(dllimport)
#endif