#pragma once
#include "Lexer.h"
#include "LiteralGetter.h"

class CLexerInput final : public ILiteralGetter
{
public:
	CLexerInput(const std::string &fileName);
	~CLexerInput();

	bool IsOpened() const;
	bool IsEOF() const;

	TokenGroup GetNextTokenGroup() const;
	
	virtual int GetChar() const override;
	virtual void FoundNextLine() const override;

private:
	const static std::set<char> whitespaces;

	FILE *m_file;
	mutable size_t m_row;

	void GetTillSinglelineComment() const;
	void GetTillMultilineComment() const;
};

