#include "stdafx.h"
#include "Semantics.h"
#include <utility>
#include <iostream>
#include <fstream>
#include "ReservedTokens.h"
#include "Param.h"
#include "VarSpec.h"
#include "Function.h"
#include "SemanticTypes.h"
#include <queue>

std::stack<CSemantics::StackType> CSemantics::m_stack;
std::stack<void*> CSemantics::m_elems;
std::stack<CSemantics::ComplexExpressionElement> CSemantics::m_typeStack;

const std::set<TokenType> CSemantics::m_forbiddenSymbols = {
	reservedTokens["["],
	reservedTokens["]"],
	reservedTokens[","],
	reservedTokens[";"],
	reservedTokens["{"],
	reservedTokens["}"],
	reservedTokens[">>"],
	reservedTokens["<<"],
	reservedTokens["="],
	reservedTokens["for"],
	reservedTokens["while"],
	reservedTokens["if"],
	reservedTokens["else"],
	reservedTokens["cin"],
	reservedTokens["cout"],
	reservedTokens["return"]
};

void CSemantics::Push(CSemantics::StackType &&element)
{
	auto curElem = std::forward<CSemantics::StackType>(element);
	if (!curElem.isToken || m_forbiddenSymbols.count(curElem.token.type) == 0)
	{
		if (!curElem.isToken && curElem.label % 2 == 1)
		{
			auto labelType = curElem.label;
			switch (labelType)
			{
			case START_CODE:
				break;
			case END_CODE:
				break;
			case FUNCTION_START:
				break;
			case FUNCTION_END:
				break;
			case FUNCTION_START_DECL:
				break;
			case FUNCTION_END_DECL:
				CreateFunction();
				break;
			case FUNCTION_START_PARAM_LIST:
				break;
			case FUNCTION_END_PARAM_LIST:
				CreateParamList();
				break;
			case FUNCTION_START_PARAM:
				break;
			case FUNCTION_END_PARAM:
				CreateParam();
				break;
			case START_EXPLIST:
				break;
			case END_EXPLIST:
				break;
			case START_DECL:
				break;
			case END_DECL:
				break;
			case START_ASSIGN:
				break;
			case END_ASSIGN:
				break;
			case START_INPUT:
				break;
			case END_INPUT:
				break;
			case START_OUTPUT:
				break;
			case END_OUTPUT:
				break;
			case START_WHILE:
				break;
			case END_WHILE:
				break;
			case START_FOR:
				break;
			case END_FOR:
				break;
			case START_CONDITION:
				break;
			case END_CONDITION:
				break;
			case START_IF:
				break;
			case END_IF:
				break;
			case START_ELSE:
				break;
			case END_ELSE:
				break;
			case START_RETURN:
				break;
			case END_RETURN:
				break;
			case START_VAR_DECL:
				break;
			case END_VAR_DECL:
				break;
			case START_ARITHMETIC_OPERATION:
				break;
			case END_ARITHMETIC_OPERATION:
				CreateArithmeticExpression();
				break;
			case START_COND_EXPR:
				break;
			case END_COND_EXPR:
				CreateConditionExpression();
				break;
			case START_ARRAY_FIRST_DIM:
				break;
			case END_ARRAY_FIRST_DIM:
				break;
			case START_ARRAY_SECOND_DIM:
				break;
			case END_ARRAY_SECOND_DIM:
				break;
			case START_MAP_DECL:
				break;
			case END_MAP_DECL:
				break;
			case START_FUNC_CALL:
				break;
			case END_FUNC_CALL:
				break;
			case START_FUNCTION_ARGUMENTS:
				break;
			case END_FUNCTION_ARGUMENTS:
				break;
			case START_FUNCTION_ARG:
				break;
			case END_FUNCTION_ARG:
				break;
			case START_FIRST_DIM:
				break;
			case END_FIRST_DIM:
				break;
			case START_SECOND_DIM:
				break;
			case END_SECOND_DIM:
				break;
			case START_VAR_DESRIBE:
				break;
			case END_VAR_DESCRIBE:
				break;
			case LABEL_NONE:
				break;
			default:
				break;
			}
		}
		else
		{
			m_stack.push(std::forward<CSemantics::StackType>(curElem));
		}
	}
}

CSemantics::StackType CSemantics::Pop()
{
	if (m_stack.empty())
	{
		throw std::runtime_error("stack pop from empty stack");
	}

	StackType elem = m_stack.top();
	m_stack.pop();

	return elem;
}

void CSemantics::LogToFile()
{
	auto stackCopy = m_stack;
	std::stack<StackType> okStack;
}

void CSemantics::CreateFunction()
{
	auto paramTable = static_cast<ParamTable*>(m_elems.top());
	m_elems.pop();

	m_stack.pop(); //miss both parenthesises
	m_stack.pop();

	std::string funcName = m_stack.top().token.tokenString;
	m_stack.pop();

	auto funcType = m_stack.top().token.type;
	VarType varType;
	switch (funcType)
	{
	case CHAR:
		varType = VarType::TYPE_CHAR;
		break;
	case STR:
		varType = VarType::TYPE_STRING;
		break;
	case FLOAT:
		varType = VarType::TYPE_FLOAT;
		break;
	case INT:
		varType = VarType::TYPE_INT;
		break;
	case VOID:
		varType = VarType::TYPE_VOID;
		break;
	default:
		assert(false);
		break;
	}
	m_stack.pop();

	FunctionTable* funcTable = new FunctionTable();
	funcTable->AddElement(new CFunction(funcName, varType, paramTable));

	m_elems.push((void*)(funcTable));
}

void CSemantics::CreateParamList()
{
	ParamTable *paramTable = new ParamTable();
	while (m_stack.top().label != Labels::FUNCTION_START_PARAM_LIST)
	{
		paramTable->AddElement(static_cast<CParam*>(m_elems.top()));
		m_elems.pop();
		m_stack.pop();
	}
	m_stack.pop();

	m_elems.push((void*)paramTable);
}

void CSemantics::CreateParam()
{
	std::string paramName = m_stack.top().token.tokenString;
	m_stack.pop();

	auto type = m_stack.top().token.type;
	VarType varType;
	switch (type)
	{
	case CHAR:
		varType = VarType::TYPE_CHAR;
		break;
	case STR:
		varType = VarType::TYPE_STRING;
		break;
	case FLOAT:
		varType = VarType::TYPE_FLOAT;
		break;
	case INT:
		varType = VarType::TYPE_INT;
		break;
	default:
		assert(false);
		break;
	}

	m_stack.pop();

	m_elems.push((void*)(new CParam(paramName, varType)));
}

void CSemantics::CreateArithmeticExpression()
{
	std::stack<StackType> tempStack;
	while (m_stack.top().label != Labels::START_ARITHMETIC_OPERATION)
	{
		tempStack.push(m_stack.top());
		m_stack.pop();
	}

	while (!tempStack.empty())
	{
		auto elem = tempStack.top();
		tempStack.pop();
		//здесь составлять ОПН и говорить генератору создавать код на основе стека
		if (elem.isToken && elem.token.type) // добавить условия, если оператор - добавлять тип
		{

		}
		else
		{
			//если тип - добавляем в стек сюда
		}
	}

	m_stack.pop();

	//добавлять в стек типов возвращаемый арифметическим выражением тип
}

void CSemantics::CreateConditionExpression()
{

}