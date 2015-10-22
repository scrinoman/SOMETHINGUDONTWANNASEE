#include "stdafx.h"
#include "Lexer.h"
#include "TokenAutomata.h"

using namespace std;

void SelfTest()
{
	assert(IsIntNumber("1LuL"));
	assert(IsIntNumber("1l"));
	assert(IsIntNumber("1154684246542468421421686876542315845412513546842"));
	assert(IsIntNumber("1ull"));
	assert(IsIntNumber("1lU"));
	assert(!IsIntNumber("1R"));
	assert(!IsIntNumber("l"));
	assert(!IsIntNumber("1e10"));

	assert(IsFloatNumber("2.9l"));
	assert(!IsFloatNumber("0;"));
	assert(IsFloatNumber(".3"));
	assert(IsFloatNumber("1."));
	assert(IsFloatNumber("1e-1"));
	assert(IsFloatNumber("2e-5"));
	assert(IsFloatNumber("15e+34"));
	assert(IsFloatNumber("2.8e1"));
	assert(!IsFloatNumber("2.8e"));
	assert(IsFloatNumber("2.9"));
	assert(IsFloatNumber("289.8e-144L"));
	assert(!IsFloatNumber("2"));
	assert(!IsFloatNumber("e12"));
	assert(!IsFloatNumber("2.3e"));
	assert(IsFloatNumber(".9"));
	
	assert(IsChar("\'a\'"));
	assert(IsChar("\'1\'"));
	assert(IsChar("\'-\'"));
	assert(IsChar("\'\\n\'"));
	assert(IsChar("\'\a\'"));
	assert(IsChar("\'\'\'"));
	assert(IsChar("\'/\'"));
	assert(IsChar("\'\\0\'"));
	assert(IsChar("\'\\\"\'"));

	assert(IsString("\"hi\""));
	assert(!IsString("\"fkdnkdf\"f\"jfdnjfkd\""));
	assert(IsString("\"fdfdff/fdf\""));
	assert(!IsString("\"hi"));
	assert(IsString("\"\""));
	assert(IsString("\"fkdjnf\\\"\""));
	assert(IsString("\"asd\"\"sad\""));


	assert(IsHEX("0xaf1"));
	assert(IsHEX("0x11111111111111111111111111111111111aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa55555555555555555555555555555bbbbbbbbbbbf"));
	assert(!IsHEX("1xa"));

	assert(IsOCT("01213"));
	assert(IsOCT("0777"));
	assert(!IsOCT("08"));
	assert(!IsOCT("11213"));

	assert(IsIdentifier("s"));
	assert(!IsIdentifier("1s"));
	assert(IsIdentifier("_s"));
	assert(IsIdentifier("s1_sdfd2bu3bruf73gfu2yg3u73237fgufy3gf76342"));
}

void OutputResult(const string &fNameOutput, const TokenTable &table)
{
	FILE *fout;
	fopen_s(&fout, fNameOutput.c_str(), "w");

	if (!fout)
	{
		return;
	}

	for (size_t i = 0; i < table.size(); ++i)
	{
		for (auto &token : table[i].tokens)
		{
			fprintf(fout, "%u %s %s\n", table[i].row, token.strType.c_str(), token.tokenString.c_str());
		}
	}

	fclose(fout);
}

int main(int argc, char* argv[])
{
	SelfTest();

	if (argc < 3)
	{
		return 1;
	}

	OutputResult(argv[2], ParseFile(argv[1]));

	return 0;
}