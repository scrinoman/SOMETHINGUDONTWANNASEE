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
std::stack<ComplexExpression> CSemantics::m_evalStack;
std::stack<VariableDescription*> CSemantics::m_varStack;
std::vector<VarElement> CSemantics::m_varTable;
std::stack<VarType> CSemantics::m_types;
int CSemantics::curScope = 0;

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

const std::map < Operator, int > CSemantics::m_operationPrior = {
	{ Operator::PLUS, 0 }, { Operator::MINUS, 0 }, { Operator::OR, 0 }, { Operator::MOD, 0 }, 
	{ Operator::MULT, 1 }, { Operator::DIV, 1 }, { Operator::AND, 1 },
	{ Operator::LOG_AND, 0 }, { Operator::LOG_OR, 0 }
};

void CSemantics::Push(CSemantics::StackType &&element)
{
	auto curElem = std::forward<CSemantics::StackType>(element);
	if (curElem.isToken)
	{
		if (curElem.token.tokenString == "{")
		{
			curScope++;
		}
		else
		{
			if (curElem.token.tokenString == "}")
			{
				curScope--;
			}
		}
	}

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
				AddVarToTable();
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
			case END_ARRAY_FIRST_DIM:
				while (m_stack.top().label != Labels::START_ARRAY_FIRST_DIM)
				{
					m_stack.pop();
				}
				break;
			case END_ARRAY_SECOND_DIM:
				while (m_stack.top().label != Labels::START_ARRAY_SECOND_DIM)
				{
					m_stack.pop();
				}
				break;
			case START_MAP_DECL:
				break;
			case END_MAP_DECL:
				break;
			case START_FUNC_CALL:
				break;
			case END_FUNC_CALL:
				break;
			case END_FUNCTION_ARGUMENTS:
			{
				FunctionArguments *args = new FunctionArguments();
				std::stack<FunctionArgument> tempStack;
				while (m_stack.top().label != Labels::START_FUNCTION_ARGUMENTS)
				{
					m_stack.pop();
					tempStack.push(m_evalStack.top());
					m_evalStack.pop();
				}
				m_stack.pop();
				while (!tempStack.empty())
				{
					args->push_back(tempStack.top());
					tempStack.pop();
				}

				m_elems.push(static_cast<void*>(args));
				break;
			}
			case END_FUNCTION_ARG:
				while (m_stack.top().label != Labels::START_FUNCTION_ARG)
				{
					m_stack.pop();
				}
				break;
			case START_FIRST_DIM:
				break;
			case END_FIRST_DIM:
				break;
			case START_SECOND_DIM:
				break;
			case END_SECOND_DIM:
				break;
			case END_VAR_DESCRIBE:
				RecognizeLeftPart();
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

int CSemantics::GetVarInTable(const std::string &varName)
{
	for (size_t i = 0; i < m_varTable.size(); ++i)
	{
		if (m_varTable[i].name == varName)
		{
			return i;
		}
	}

	return -1;
}

void CSemantics::RecognizeArrayPart()
{
	auto top = m_stack.top();
	bool hasFirst = false, hasSecond = false;
	ComplexExpression exp1, exp2;
	if (top.label == Labels::START_ARRAY_SECOND_DIM)
	{
		hasSecond = true;
		exp2 = m_evalStack.top();
		m_evalStack.pop();
		m_stack.pop();
	}

	m_stack.pop();
	hasFirst = true;
	exp1 = m_evalStack.top();
	m_evalStack.pop();

	VariableDescription *desc = hasSecond ? new VariableDescription(GetVarInTable(m_stack.top().token.tokenString), exp1, exp2) : 
											new VariableDescription(GetVarInTable(m_stack.top().token.tokenString), exp1);
	m_varStack.push(desc);
}

void CSemantics::RecognizeFuncCall()
{
	m_stack.pop();
	FunctionCall *call = new FunctionCall();
	call->args = static_cast<FunctionArguments*>(m_elems.top());
	m_elems.pop();
	m_stack.pop();
	auto top = m_stack.top();

	VariableDescription *desc = new VariableDescription();
	desc->func = call;
	desc->isFunctionCall = true;
	m_stack.pop();

	m_varStack.push(desc);
}

void CSemantics::RecognizeSimpleVar()
{
	auto top = m_stack.top();
	m_stack.pop();

	VariableDescription *desc = new VariableDescription(GetVarInTable(top.token.tokenString));
	m_varStack.push(desc);
}

void CSemantics::RecognizeLeftPart()
{
	auto top = m_stack.top();
	if (top.isToken && top.token.tokenString == ")")
	{
		RecognizeFuncCall();
		return;
	}

	if (!top.isToken && (top.label == Labels::START_ARRAY_FIRST_DIM || top.label == Labels::START_ARRAY_SECOND_DIM))
	{
		RecognizeArrayPart();
		return;
	}

	RecognizeSimpleVar();
}

void CSemantics::AddVarToTable()
{
	auto top = m_stack.top();
	VarElement newVarElement;
	if (!top.isToken)
	{
		if (top.label == Labels::START_ARRAY_SECOND_DIM)
		{
			newVarElement.hasSecondDim = true;
			auto curType = m_types.top();
			newVarElement.secondDimType = curType;
			m_types.pop();
			m_stack.pop();

			top = m_stack.top();
		}

		if (top.label == Labels::START_ARRAY_FIRST_DIM)
		{
			newVarElement.hasFirstDim = true;
			auto curType = m_types.top();
			newVarElement.firstDimType = curType;
			m_types.pop();
			m_stack.pop();

			top = m_stack.top();
		}
	}

	newVarElement.name = top.token.tokenString;
	newVarElement.scope = curScope;
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

	newVarElement.type = varType;
	
	m_varTable.push_back(newVarElement);
}

Operator CSemantics::GetOperatorByString(const std::string &opString)
{
	if (opString == "+") return Operator::PLUS;
	if (opString == "-") return Operator::MINUS;
	if (opString == "*") return Operator::MULT;
	if (opString == "/") return Operator::DIV;
	if (opString == "%") return Operator::MOD;
	if (opString == "|") return Operator::OR;
	if (opString == "&") return Operator::AND;
	//if (opString == "-") return Operator::MINUS;
}

//пока не проверяется унарный минус
void CSemantics::CreateArithmeticExpression()
{
	ComplexExpression newExpr;
	std::stack<ComplexExpressionElement> reverseNotationStack;

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
		if (elem.isToken) // добавить условия, если оператор - добавлять тип
		{
			if (elem.token.type == TokenType::INTEGER_DEC_NUMBER || elem.token.type == TokenType::FLOAT_NUMBER ||
				elem.token.type == TokenType::STRING || elem.token.type == TokenType::CHARACTER)
			{
				newExpr.push_back(elem.token);
			}
			else
			{
				if (elem.token.tokenString == "(")
				{
					reverseNotationStack.push(elem.token);
				}
				else
				{
					if (elem.token.tokenString == ")")
					{
						while (!reverseNotationStack.top().isConst || reverseNotationStack.top().constToken.tokenString != "(")
						{
							auto newElem = reverseNotationStack.top();
							newExpr.push_back(newElem);
							reverseNotationStack.pop();
						}
						reverseNotationStack.pop();
					}
					else //must be operator
					{
						auto opString = elem.token.tokenString;
						
						while (!reverseNotationStack.empty() && reverseNotationStack.top().isOperator)
						{
							auto lastOP = reverseNotationStack.top();
							if (m_operationPrior.at(lastOP.op) >= m_operationPrior.at(GetOperatorByString(opString)))
							{
								newExpr.push_back(reverseNotationStack.top());
								reverseNotationStack.pop();
							}
							else
							{
								break;
							}
						}

						reverseNotationStack.push(GetOperatorByString(elem.token.tokenString));
					}
				}
			}
		}
		else
		{
			if (elem.label == Labels::START_VAR_DESRIBE)
			{
				auto varPoint = m_varStack.top();
				m_varStack.pop();
				newExpr.push_back(varPoint);
			}
		}
	}

	while (!reverseNotationStack.empty())
	{
		newExpr.push_back(reverseNotationStack.top());
		reverseNotationStack.pop();
	}

	m_evalStack.push(newExpr);
	//добавлять в стек типов возвращаемый арифметическим выражением тип
}

void CSemantics::CreateConditionExpression()
{

}