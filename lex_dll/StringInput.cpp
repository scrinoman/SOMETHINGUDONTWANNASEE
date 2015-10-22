#include "stdafx.h"
#include "StringInput.h"

using namespace std;

CStringInput::CStringInput(const std::string &s)
	:m_string(s), m_index(0)
{
}

bool CStringInput::IsEnd() const
{
	return m_index >= m_string.length();
}

void CStringInput::IncIndex(size_t inc) const
{
	m_index += inc;
}

int CStringInput::GetChar() const
{
	if (!IsEnd())
	{
		return m_string[m_index++];
	}

	return END_OF_LITERAL_FORMAT;
}

void CStringInput::FoundNextLine() const
{
}