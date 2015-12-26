#include "stdafx.h"
#include "SyntaxTable.h"

CSyntaxTable::CSyntaxTable(const std::string &name, SYNTAX_ELEM_TYPE type)
	:m_name(name), m_type(type), m_child(nullptr), m_next(nullptr)
{
}

CSyntaxTable::~CSyntaxTable()
{
	if (m_next)
	{
		delete m_next;
	}

	if (m_child)
	{
		delete m_child;
	}
}

void CSyntaxTable::AddChild(Node child)
{
	if (m_child == nullptr)
	{
		m_child = child;
	}
	else
	{
		m_child->AddNext(child);
	}
}

void CSyntaxTable::AddNext(Node next)
{
	if (m_next == nullptr)
	{
		m_next = next;
	}
	else
	{
		m_next->AddNext(next);
	}
}

std::string CSyntaxTable::GetName() const
{
	return m_name;
}

CSyntaxTable::SYNTAX_ELEM_TYPE CSyntaxTable::GetType() const
{
	return m_type;
}