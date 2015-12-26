#pragma once
#include "LiteralGetter.h"

class CStringInput final : public ILiteralGetter 
{
public:
	CStringInput(const std::string &s);

	bool IsEnd() const;
	virtual int GetChar() const override;
	virtual void FoundNextLine() const override;
	void IncIndex(size_t inc) const;

private:
	const std::string &m_string;
	mutable size_t m_index;
};

