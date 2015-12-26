#include "stdafx.h"
#include "LexerInput.h"

using namespace std;

const std::set<char> CLexerInput::whitespaces = { ' ', '\t', '\v', '\f', '\n' };

CLexerInput::CLexerInput(const string &fileName)
	:m_row(1)
{
	fopen_s(&m_file, fileName.c_str(), "r");
}

CLexerInput::~CLexerInput()
{
	if (m_file)
	{
		fclose(m_file);
	}
}

bool CLexerInput::IsOpened() const
{
	return m_file != nullptr;
}

bool CLexerInput::IsEOF() const
{
	return feof(m_file) != 0;
}

TokenGroup CLexerInput::GetNextTokenGroup() const
{
	string res = "";
	bool wasSlash = false;
	size_t curRow = 0;
	char c;

	while ((c = fgetc(m_file)) != EOF)
	{
		if (c == '\n')
		{
			m_row++;
		}
		
		if (c == '#')
		{
			GetTillSinglelineComment(); //pre-proc = single-line commentary
			continue;
		}

		if (wasSlash)
		{
			if (c == '/' || c == '*')
			{
				c == '/' ? GetTillSinglelineComment() : GetTillMultilineComment();
				wasSlash = false;
				if (res.length() > 0)
				{
					break;
				}
				continue;
			}

			if (curRow == 0)
			{
				curRow = m_row;
			}
			res += '/';

			wasSlash = false;
		}
		else
		{
			wasSlash = c == '/';
			if (wasSlash)
			{
				continue;
			}
		}

		if (c == '\'' || c == '\"')
		{
			if (curRow == 0)
			{
				curRow = m_row;
			}

			res += c;
			res += c == '\'' ? GetTillChar() : GetTillString();
			
			break;
		}

		if (whitespaces.count(c) > 0)
		{
			if (res.length() > 0)
			{
				break;
			}
			continue;
		}

		if (curRow == 0)
		{
			curRow = m_row;
		}
		res += c;
	}

	return TokenGroup(move(res), curRow);
}

void CLexerInput::GetTillSinglelineComment() const
{
	char c;
	while ((c = fgetc(m_file)) != '\n' && !feof(m_file)){}
	m_row++;
}

void CLexerInput::GetTillMultilineComment() const
{
	char cur, prev = '\0';
	while ((cur = fgetc(m_file)) != EOF)
	{
		if (cur == '\n')
		{
			m_row++;
		}

		if (cur == '/' && prev == '*')
		{
			break;
		}

		prev = cur;
	}
}

int CLexerInput::GetChar() const
{
	return fgetc(m_file);
}

void CLexerInput::FoundNextLine() const
{
	m_row++;
}