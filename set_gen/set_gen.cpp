#include "stdafx.h"
#include "reservedTokens.h"
#include "TokenType.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("input.txt");
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
		string type;// , isLeft, isLast;
		iss >> type;
		//iss >> isLeft >> isLast >> type;
		//isLeft == "1" ? isLeft = "true" : "false";
		//isLast == "1" ? isLast = "true" : "false";
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

