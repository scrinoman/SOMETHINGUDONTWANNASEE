// lex_dll.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include "lex_dll.h"


// ������ ���������������� ����������
LEX_DLL_API int nlex_dll=0;

// ������ ���������������� �������.
LEX_DLL_API int fnlex_dll(void)
{
	return 42;
}

// ����������� ��� ����������������� ������.
// ��. ����������� ������ � lex_dll.h
Clex_dll::Clex_dll()
{
	return;
}
