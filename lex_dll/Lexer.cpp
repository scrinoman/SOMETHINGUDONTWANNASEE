#include "stdafx.h"
#include "Lexer.h"
#include "ReservedTokens.h"
#include "TokenFunc.h"
#include "LexerInput.h"
#include "StringInput.h"
#include "LiteralGetter.h"

using namespace std;

set<string>::const_iterator FindDelimiter(const string &s, size_t start)
{
	for (size_t i = 3; i > 0; --i)
	{
		string potentialDelim = s.substr(start, i);
		auto it = delimiters.find(potentialDelim);
		if (it != delimiters.cend())
		{
			return it;
		}
	}

	return delimiters.cend();
}

bool DetermineTokenType(const string &token, TokenType &newTokenType)
{
	static const vector<TokenType> complexTokens =
		{ TokenType::IDENTIFIER, TokenType::INTEGER_DEC_NUMBER, /*TokenType::INTEGER_HEX_NUMBER, TokenType::INTEGER_OCT_NUMBER,*/
		TokenType::FLOAT_NUMBER, TokenType::CHARACTER, TokenType::STRING };

	if (reservedTokens.count(token) > 0)
	{
		newTokenType = reservedTokens[token];
		return true;
	}
	else
	{
		for (auto &tokenType : complexTokens)
		{
			if (tokenFunc[tokenType](token))
			{
				newTokenType = tokenType;
				return true;
			}
		}
	}

	return false;
}

bool DetermineToken(const string &token, Token &newToken)
{
	if (token != "")
	{
		TokenType type;
		if (DetermineTokenType(token, type))
		{
			newToken = Token(token, type);
			return true;
		}
	}
	else
	{
		return true;
	}

	newToken = Token(token, TokenType::ERROR);
	return false;
}

vector<Token> RecognizeTokens(const string &token)
{
	vector<Token> res;
	string curToken = "";
	bool found = false;
	CStringInput strToken(token);
	size_t i = 0;

	while (!strToken.IsEnd())
	{
		char c = strToken.GetChar();
		set<string>::const_iterator it = delimiters.cend();
		if (c == '\'' || c == '\"')
		{
			curToken += c;
			string newLiteral = c == '\'' ? strToken.GetTillChar() : strToken.GetTillString();
			i += newLiteral.length();
			curToken += newLiteral;
		}
		else
		{
			if ((it = FindDelimiter(token, i)) == delimiters.cend())
			{
				curToken += c;
			}
		}
		i++;
		
		if (it != delimiters.end())
		{
			Token newToken("", TokenType::ERROR);
			found = DetermineToken(curToken, newToken);

			if (*it == "+" || *it == "-")
			{
				if (curToken != "" && newToken.type != TokenType::IDENTIFIER)
				{
					curToken += c;
					continue;
				}
			}

			if (*it == ".")
			{
				if (newToken.type != TokenType::IDENTIFIER)
				{
					curToken += c;
					continue;
				}
			}

			if (curToken != "")
			{
				res.push_back(newToken);
			}

			if (!found)
			{
				return res;
			}

			res.push_back(Token(*it, reservedTokens[*it]));
			i += (it->length() - 1);
			strToken.IncIndex(it->length() - 1);
			curToken = "";
		}
	}
	
	if (curToken != "")
	{
		Token newToken("", TokenType::ERROR);
		found = DetermineToken(curToken, newToken);
		res.push_back(newToken);
	}

	return res;
}

LEX_DLL_API TokenTable ParseFile(const string &fNameInput)
{
	TokenTable result;
	CLexerInput lexer(fNameInput);
	if (!lexer.IsOpened())
	{
		return result;
	}

	while (!lexer.IsEOF())
	{
		auto &tokenGroup = lexer.GetNextTokenGroup();
		result.push_back(TokenLine(RecognizeTokens(tokenGroup.tokenString), tokenGroup.row));
		
		if (result.size() > 0)
		{
			auto &curTokens = result[result.size() - 1].tokens;
			if (curTokens.size() > 0 && curTokens[curTokens.size() - 1].type == TokenType::ERROR)
			{
				break;
			}
		}
	}

	return result;
}