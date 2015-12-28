#include "stdafx.h"
#include "Lexer.h"
#include "ReservedTokens.h"
#include "TokenFunc.h"
#include "LexerInput.h"
#include "StringInput.h"
#include "LiteralGetter.h"

using namespace std;

static const vector<TokenType> complexTokens =
	{ TokenType::IDENTIFIER, TokenType::INTEGER_DEC_NUMBER, TokenType::FLOAT_NUMBER,
	  TokenType::CHARACTER, TokenType::STRING };

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

vector<Token> RecognizeTokens(const string &token/*, TokenType prevTokenType*/)
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
				/*bool isPrevTokenTypeInComplex = false;
				if (found && newToken.type != TokenType::ERROR)
				{
					prevTokenType = newToken.type;
				}
				for each (TokenType type in complexTokens)  // roflovelosiped
				{
					if (prevTokenType == type)
					{
						isPrevTokenTypeInComplex = true;
						break;
					}
				}
				if (!isPrevTokenTypeInComplex)
				{
					curToken += c;
					continue;
				}
				*/
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

			//prevTokenType = reservedTokens[*it];
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

vector<Token> FillNewTokens(TokenLine const &line, size_t replacingIndex)
{
	vector<Token> tokens;
	size_t i = 0;
	while (i < line.tokens.size())
	{
		if (i == replacingIndex)
		{
			i++;
			string tokenString = "-" + line.tokens[i].tokenString;
			Token token = Token(tokenString, line.tokens[i].type);
			tokens.push_back(token);
		}
		else
		{
			tokens.push_back(line.tokens[i]);
		}
		i++;
	}
	return tokens;
}

void ParseTokenLine(TokenLine &line)  // append unary minus to consts if needed
{
	for (size_t i = 0; i < line.tokens.size(); i++)
	{
		if (line.tokens[i].type == TokenType::MINUS && i < line.tokens.size() - 1 &&
			(line.tokens[i + 1].type == TokenType::INTEGER_DEC_NUMBER || line.tokens[i + 1].type == TokenType::FLOAT_NUMBER || line.tokens[i + 1].type == TokenType::FLOAT))
		{
			line.tokens = FillNewTokens(line, i);
		}
	}
}

LEX_DLL_API LexerResult ParseFile(const string &fNameInput)
{
	TokenTable table;
	CLexerInput lexer(fNameInput);
	bool isError = false;

	if (!lexer.IsOpened())
	{
		return LexerResult(table, true, string("Error occured while opening file ") + fNameInput);
	}

	string errString = "";
	//TokenType prevTokenType = TokenType::INT; // roflokostil
	while (!lexer.IsEOF())
	{
		auto &tokenGroup = lexer.GetNextTokenGroup();
		TokenLine newTokenLine = TokenLine(RecognizeTokens(tokenGroup.tokenString), tokenGroup.row);
		ParseTokenLine(newTokenLine);
		//prevTokenType = newTokenLine.tokens.back().type;
		table.push_back(newTokenLine);
		
		if (table.size() > 0)
		{
			auto &curTokens = table[table.size() - 1].tokens;
			if (curTokens.size() > 0 && curTokens[curTokens.size() - 1].type == TokenType::ERROR)
			{
				isError = true;
				errString = "Strange lexem at " + to_string(tokenGroup.row) + 
							" (" + curTokens[curTokens.size() - 1].tokenString + ")";
				break;
			}
		}
	}

	return LexerResult(table, isError, errString);
}