#include "stdafx.h"
#include "reservedTokens.h"
#include "TokenType.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin(argv[1]);
	ofstream fout("result.txt");

	if (argv[2] == _T("t"))
	{
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
	}
	else
	{
		string line;
		while (getline(fin, line))
		{
			if (line == "")
			{
				continue;
			}

			fout << "CSLRRow( {";
			istringstream iss(line);
			string type;
			while (iss >> type)
			{
				fout << "{\"" << type << "\", CSLRRow::Action(CSLRRow::Action::ActionType::";
				string lexem;
				iss >> lexem;
				bool needNext = true;
				if (lexem[0] == 's')
				{
					fout << "SHIFT";
				}
				else
				{
					if (lexem[0] == 'r')
					{
						fout << "REDUCE";
					}
					else
					{
						if (lexem[0] == 'g')
						{
							fout << "GOTO";
						}
						else
						{
							needNext = false;
							fout << "ACCEPTED";
						}
					}
				}
				if (needNext)
				{
					fout << ", " << lexem.substr(1, lexem.length() - 1);
				}
				else
				{
					fout << ", 0";
				}
				fout << ") }";

				if (!iss.eof())
				{
					fout << ", ";
				}
			}
			fout << "})," << std::endl;
		}
	}
	

	return 0;
}

