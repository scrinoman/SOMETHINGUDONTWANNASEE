// ����������� ���� ���� ifdef - ��� ����������� ����� �������� ��������, ���������� ��������� 
// �������� �� ��������� DLL. ��� ����� ������ DLL �������������� � �������������� ������� LEX_DLL_EXPORTS,
// � ��������� ������. ���� ������ �� ������ ���� ��������� � �����-���� �������
// ������������ ������ DLL. ��������� ����� ����� ������ ������, ��� �������� ����� �������� ������ ����, ����� 
// ������� LEX_DLL_API ��� ��������������� �� DLL, ����� ��� ������ DLL ����� �������,
// ������������ ������ ��������, ��� ����������������.
#ifdef LEX_DLL_EXPORTS
#define LEX_DLL_API __declspec(dllexport)
#else
#define LEX_DLL_API __declspec(dllimport)
#endif

//// ���� ����� ������������� �� lex_dll.dll
//class LEX_DLL_API Clex_dll {
//public:
//	Clex_dll(void);
//	// TODO: �������� ����� ���� ������.
//};
//
//extern LEX_DLL_API int nlex_dll;
//
//LEX_DLL_API int fnlex_dll(void);
