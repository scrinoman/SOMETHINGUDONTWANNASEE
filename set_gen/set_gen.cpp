#include "stdafx.h"
#include "reservedTokens.h"
#include "TokenType.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin(argv[1]);
	ofstream fout("result.txt");

	string line;
	while (getline(fin, line))
	{
		if (line == "")
		{
			continue;
		}

		fout << "CRT({ CRR(";
		istringstream iss(line);
		string type;
		iss >> type;
		fout << "\"" << type << "\", {";
		string lexem;
		while (iss >> lexem)
		{
			if (lexem == "id")
			{
				fout << "TokenType(" << TokenType::IDENTIFIER << ")";
			}
			else
			{
				if (lexem == "const")
				{
					fout << "TokenType(" << TokenType::INTEGER_DEC_NUMBER << "), " << "TokenType(" << TokenType::FLOAT_NUMBER << "), "
						<< "TokenType(" << TokenType::CHARACTER << "), " << "TokenType(" << TokenType::STRING << ")";
				}
				else
				{

					fout << "TokenType(" << reservedTokens[lexem] << ")";
				}
			}

			if (!iss.eof())
			{
				fout << ", ";
			}
		}

		fout << " }, 0)}), " << endl;
	}

	return 0;
}

