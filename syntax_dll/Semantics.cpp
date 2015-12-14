#include "stdafx.h"
#include "Semantics.h"
#include <utility>
#include <iostream>
#include <fstream>

std::stack<CSemantics::StackType> CSemantics::m_stack;

void CSemantics::Push(CSemantics::StackType &&element)
{
	m_stack.push(std::forward<CSemantics::StackType>(element));
}

CSemantics::StackType CSemantics::Pop()
{
	if (m_stack.empty())
	{
		//throw std::runtime_error("stack pop from empty stack");
	}

	StackType elem = m_stack.top();
	m_stack.pop();

	return elem;
}

void CSemantics::LogToFile()
{
	std::ofstream fout("stack.log.txt");
	auto stackCopy = m_stack;
	std::stack<StackType> okStack;
	while (!stackCopy.empty())
	{
		okStack.push(stackCopy.top());
		stackCopy.pop();
	}

	while (!okStack.empty())
	{
		fout << okStack.top().label << std::endl;
		okStack.pop();
	}
}