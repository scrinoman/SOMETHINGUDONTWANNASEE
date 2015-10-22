#include "stdafx.h"
#include "LiteralGetter.h"

using namespace std;

const int ILiteralGetter::END_OF_LITERAL_FORMAT = -1;

string ILiteralGetter::GetSequenceTillCharacter(char endChar) const
{
	string res = "";
	bool wasSlash = false;
	int c;
	char prev = '\0';
	while ((c = GetChar()) != END_OF_LITERAL_FORMAT)
	{
		if (c == '\n')
		{
			FoundNextLine();
			if (endChar != '\"' || prev != '\\')
			{
				break;
			}
		}

		res += c;

		if (c == endChar && !wasSlash)
		{
			break;
		}

		if (c == '\\')
		{
			wasSlash = !wasSlash;
		}
		else
		{
			wasSlash = false;
		}

		prev = c;
	}

	return res;
}

string ILiteralGetter::GetTillChar() const
{
	return GetSequenceTillCharacter('\'');
}

string ILiteralGetter::GetTillString() const
{
	return GetSequenceTillCharacter('\"');
}