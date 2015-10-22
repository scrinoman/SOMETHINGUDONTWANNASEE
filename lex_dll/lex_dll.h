// Приведенный ниже блок ifdef - это стандартный метод создания макросов, упрощающий процедуру 
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа LEX_DLL_EXPORTS,
// в командной строке. Этот символ не должен быть определен в каком-либо проекте
// использующем данную DLL. Благодаря этому любой другой проект, чьи исходные файлы включают данный файл, видит 
// функции LEX_DLL_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef LEX_DLL_EXPORTS
#define LEX_DLL_API __declspec(dllexport)
#else
#define LEX_DLL_API __declspec(dllimport)
#endif

//// Этот класс экспортирован из lex_dll.dll
//class LEX_DLL_API Clex_dll {
//public:
//	Clex_dll(void);
//	// TODO: Добавьте здесь свои методы.
//};
//
//extern LEX_DLL_API int nlex_dll;
//
//LEX_DLL_API int fnlex_dll(void);
