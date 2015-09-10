#pragma once
class ILiteralGetter
{
public:	
	virtual int GetChar() const = 0;
	virtual void FoundNextLine() const = 0;

	virtual std::string GetTillChar() const;
	virtual std::string GetTillString() const;

protected:
	static const int END_OF_LITERAL_FORMAT;

private:
	virtual std::string GetSequenceTillCharacter(char) const;
};

