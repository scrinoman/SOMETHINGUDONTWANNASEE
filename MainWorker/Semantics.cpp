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

bool CSemantics::m_error = false;
std::stack<CSemantics::StackType> CSemantics::m_stack;
std::stack<void*> CSemantics::m_elems;
std::stack<ComplexExpression> CSemantics::m_evalStack;
std::stack<BooleanComplexExpression> CSemantics::m_boolStack;
std::stack<VariableDescription*> CSemantics::m_varStack;
std::vector<VarElement> CSemantics::m_curVarTable;
std::vector<VarElement> CSemantics::m_varTable;
std::stack<VarType> CSemantics::m_types;
std::vector<FunctionTable*> CSemantics::m_funcTables;
std::ofstream CSemantics::cmdWriter("D:\\Code\\3_1\\git\\SOMETHINGUDONTWANNASEE\\Debug\\cmdLog.txt");
int CSemantics::curScope = 0;
bool CSemantics::functionHasReturn = false;
bool CSemantics::hasEntryPoint = false;

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
	{ Operator::MULT, 1 }, { Operator::DIV, 1 }, { Operator::AND, 1 }, {Operator::UNARY_MINUS, 2},
	{ Operator::LOG_AND, 0 }, { Operator::LOG_OR, 0 },
	{ Operator::GREATER, 1 }, { Operator::GREATER_OR_EQUAL, 1 }, { Operator::LESS, 1 }, { Operator::LESS_OR_EQUAL, 1 },
	{ Operator::EQUAL, 1 }, { Operator::NOT_EQUAL, 1 }, { Operator::NOT, 2 }
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
				while (!m_curVarTable.empty() && m_curVarTable.back().scope == curScope)
				{
					m_curVarTable.pop_back();
				}
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
			case END_CODE:
				break;
			case FUNCTION_END:
			{
				if (functionHasReturn || m_funcTables.back()->GetElement(0)->GetType() == VarType::TYPE_VOID)
				{
					cmdWriter << "func_end" << std::endl;
					functionHasReturn = false;
				}
				else
				{
					m_error = 1;
					std::cout << "Function might has no return statement" << std::endl;
				}
				break;
			}	
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
			case END_DECL:
			{
				if (m_stack.top().label != Labels::START_VAR_DECL && m_stack.top().label != Labels::START_MAP_DECL)
				{
					auto newVar = m_curVarTable.back();
					if (newVar.hasFirstDim)
					{
						m_error = 1;
						std::cout << "Assignment to array " << newVar.name;
						break;
					}
					else
					{
						m_varStack.push(new VariableDescription(m_curVarTable.size() - 1)); //opasno !
						LogOpAction();
						cmdWriter << std::endl;
					}
				}
				break;
			}
			case END_ASSIGN:
			{
				LogOpAction();
				cmdWriter << std::endl;
				break;
			}
			case END_INPUT:
			{
				cmdWriter << "input ";
				auto desc = m_varStack.top();
				m_varStack.pop();
				if (desc->hasFirstDim)
				{
					if (desc->hasSecondDim)
					{
						cmdWriter << "2 ";
					}
					else
					{
						cmdWriter << "1 ";
					}
				}
				else
				{
					cmdWriter << "0 ";
				}
				LogDescription(*desc);
				cmdWriter << std::endl;
				break;
			}
			case END_OUTPUT:
			{
				cmdWriter << "output ";
				LogExpression(m_evalStack.top());
				m_evalStack.pop();
				m_types.pop();
				cmdWriter << std::endl;
				break;
			}
			case END_WHILE:
				cmdWriter << "end_while" << std::endl;
				break;
			case END_WHILE_COND:
			{
				cmdWriter << "while ";
				LogBooleanExpression(m_boolStack.top());
				m_boolStack.pop();
				cmdWriter << std::endl;
				break;
			}
			case START_FOR:
				break;
			case END_FOR:
			{
				cmdWriter << "end_for ";
				while (m_stack.top().label != Labels::START_FOR_CHANGE)
				{
					m_stack.pop();
				}
				m_stack.pop();
				
				auto varName = m_stack.top().token.tokenString;
				m_stack.pop();
				int var = GetVarInTable(varName);
				if (var != -1)
				{
					m_varStack.push(new VariableDescription(var));
					LogOpAction();
					cmdWriter << std::endl;
				}

				while (!m_curVarTable.empty() && m_curVarTable.back().scope == curScope)
				{
					m_curVarTable.pop_back();
				}

				curScope--;

				break;
			}
			case END_CONDITION:
			{
				if (m_stack.top().label == Labels::START_ELSE)
				{
					cmdWriter << "end_else" << std::endl;
				}
				else
				{
					cmdWriter << "end_if end" << std::endl;
				}
				break;
			}
			case END_IF_DECL:
			{
				cmdWriter << "if ";
				LogBooleanExpression(m_boolStack.top());
				m_boolStack.pop();
				cmdWriter << std::endl;
				break;
			}
			case END_ELSE:
			{
				while (m_stack.top().label != Labels::START_ELSE)
				{
					m_stack.pop();
				}
				break;
			}
			case END_RETURN:
			{
				if (m_stack.top().label == Labels::START_RETURN)
				{
					if (m_funcTables.back()->GetElement(0)->GetType() != VarType::TYPE_VOID)
					{
						m_error = 1;
						std::cout << "Expected statement after return" << std::endl;
						break;
					}
					else
					{
						if (curScope == 1)
						{
							functionHasReturn = true;
						}

						cmdWriter << "return empty" << std::endl;
					}
				}
				else
				{
					auto exp = m_evalStack.top();
					if (IsCompatibleTypes(m_funcTables.back()->GetElement(0)->GetType(), exp.type))
					{
						if (curScope == 1)
						{
							functionHasReturn = true;
						}
						cmdWriter << "return ";
						LogVarType(m_funcTables.back()->GetElement(0)->GetType());
						LogVarType(exp.type);
						LogExpression(exp);
						m_evalStack.pop();
						m_types.pop();
						cmdWriter << std::endl;
					}
					else
					{
						m_error = 1;
						std::cout << "Wrong return type" << std::endl;
						break;
					}
					
				}
				break;
			}
			case END_VAR_DECL:
			{
				AddVarToTable();
				cmdWriter << std::endl;
				break;
			}
			case END_ARITHMETIC_OPERATION:
				CreateArithmeticExpression();
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
			case END_FUNCTION_ARGUMENTS:
			{
				std::stack<FunctionArgument> tempStack;
				while (m_stack.top().label != Labels::START_FUNCTION_ARGUMENTS)
				{
					if (m_stack.top().label == Labels::END_FUNCTION_ARG)
					{
						tempStack.emplace(m_evalStack.top(), m_types.top());
						m_evalStack.pop();
						m_types.pop();
					}
					m_stack.pop();
				}
				m_stack.pop();

				FunctionArguments *args = new FunctionArguments();
				while (!tempStack.empty())
				{
					args->push_back(tempStack.top());
					tempStack.pop();
				}

				m_elems.push(static_cast<void*>(args));
				break;
			}
			case END_FUNCTION_ARG:
			{
				while (m_stack.top().label != Labels::START_FUNCTION_ARG)
				{
					m_stack.pop();
				}
				m_stack.pop();
				m_stack.push(Labels::END_FUNCTION_ARG); // просто костыль для пустого списка аргументов (непарный енд)
				break;
			}
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
			case END_FOR_VAR_DESC:
			{
				curScope++;
				cmdWriter << "for ";
				AddVarToTable();
				if (m_curVarTable.back().hasFirstDim)
				{
					m_error = true;
					std::cout << "For initializer must be variable, not array";
					break;
				}
				m_varStack.push(new VariableDescription(m_curVarTable.size() - 1)); //opasno !
				break;
			}
			case END_FOR_VAR_EXPR:
			{
				LogOpAction();
				cmdWriter << " ";
				break;
			}
			case END_FOR_COND:
			{
				LogBooleanExpression(m_boolStack.top());
				m_boolStack.pop();
				cmdWriter << std::endl;
				break;
			}
			case END_FOR_CHANGE:
			{				
				break;
			}
			default:
				break;
			}
		}
		else
		{
			if (curElem.label == Labels::START_ELSE)
			{
				cmdWriter << "end_if else" << std::endl << "else" << std::endl;
			}
			m_stack.push(std::forward<CSemantics::StackType>(curElem));
		}
	}
}

bool CSemantics::IsCompatibleTypes(VarType needType, VarType gotType)
{
	return (needType == gotType || (needType == VarType::TYPE_FLOAT && gotType == VarType::TYPE_INT));
}

void CSemantics::CloseLogger()
{
	cmdWriter.close();
}

bool CSemantics::LastCheck()
{
	return hasEntryPoint;
}

void CSemantics::LogVarType(VarType type)
{
	switch (type)
	{
	case VarType::TYPE_VOID:
		cmdWriter << "void";
		break;
	case VarType::TYPE_INT:
		cmdWriter << "int";
		break;
	case VarType::TYPE_CHAR:
		cmdWriter << "char";
		break;
	case VarType::TYPE_FLOAT:
		cmdWriter << "float";
		break;
	case VarType::TYPE_STRING:
		cmdWriter << "string";
		break;
	case VarType::TYPE_BOOL:
		cmdWriter << "bool";
		break;
	default:
		break;
	}

	cmdWriter << " ";
}

std::string CSemantics::GetOPString(Operator op)
{
	switch (op)
	{
	case Operator::PLUS:
		return "+";
		break;
	case Operator::MINUS:
		return "-";
		break;
	case Operator::OR:
		return "|";
		break;
	case Operator::MOD:
		return "%";
		break;
	case Operator::MULT:
		return "*";
		break;
	case Operator::DIV:
		return "/";
		break;
	case Operator::UNARY_MINUS:
		return "unary_minus";
		break;
	case Operator::AND:
		return "&";
		break;
	case Operator::LOG_AND:
		return "&&";
		break;
	case Operator::LOG_OR:
		return "||";
		break;
	case Operator::GREATER:
		return ">";
		break;
	case Operator::LESS:
		return "<";
		break;
	case Operator::EQUAL:
		return "==";
		break;
	case Operator::NOT_EQUAL:
		return "!=";
		break;
	case Operator::GREATER_OR_EQUAL:
		return ">=";
		break;
	case Operator::LESS_OR_EQUAL:
		return "<=";
		break;
	case Operator::NOT:
		return "!";
		break;
	default:
		break;
	}
}

void CSemantics::LogOpAction()
{
	cmdWriter << "assign ";
	auto desc = m_varStack.top();
	m_varStack.pop();
	if (desc->hasFirstDim)
	{
		if (desc->hasSecondDim)
		{
			cmdWriter << "2 ";
		}
		else
		{
			cmdWriter << "1 ";
		}
		LogDescription(*desc);
		cmdWriter << " ";
		LogExpression(m_evalStack.top());
		m_evalStack.pop();
	}
	else
	{
		cmdWriter << "0 ";
		LogExpression(m_evalStack.top());
		m_evalStack.pop();
		cmdWriter << " ";
		LogDescription(*desc);
	}
}

void CSemantics::LogBooleanExpression(const BooleanComplexExpression &exp)
{
	cmdWriter << "condition ";
	cmdWriter << exp.size() << " ";
	for (size_t i = 0; i < exp.size(); ++i)
	{
		if (exp[i].type == BooleanExpressionElement::ExpType::EXPRESSION_OPERATOR)
		{
			cmdWriter << "op " << GetOPString(exp[i].op);
		}
		else
		{
			if (exp[i].type == BooleanExpressionElement::ExpType::EXPRESSION_CONST)
			{
				cmdWriter << exp[i].constToken.strType << " " << exp[i].constToken.tokenString;
			}
			else
			{
				LogExpression(exp[i].elem);
			}
		}

		if (i < exp.size() - 1)
		{
			cmdWriter << " ";
		}
	}
}

void CSemantics::LogDescription(const VariableDescription &desc)
{
	if (desc.isFunctionCall) //func name args_count <args>
	{
		cmdWriter << "funcCall ";
		cmdWriter << desc.func->funcPointer << " ";
		size_t count = (*(desc.func->args)).size();
		cmdWriter << count << " ";
		for (size_t i = 0; i < count; ++i)
		{
			LogExpression((*(desc.func->args))[i].exp);
			cmdWriter << " ";
		}
	}
	else
	{
		cmdWriter << "variable "; //var dim pointer type desc
		int dim = 0;
		if (desc.hasFirstDim) dim++;
		if (desc.hasSecondDim) dim++;
		cmdWriter << dim << " ";
		cmdWriter << desc.pointer << " ";
		LogVarType(m_curVarTable[desc.pointer].type);
		if (desc.hasFirstDim)
		{
			cmdWriter << "fd ";
			LogExpression(desc.firstDim);
		}

		if (desc.hasSecondDim)
		{
			cmdWriter << " sd ";
			LogExpression(desc.secondDim);
		}
	}
}

void CSemantics::LogExpression(const ComplexExpression &exp)
{
	cmdWriter << "expression ";
	LogVarType(exp.type);
	cmdWriter << exp.elems.size() << " ";
	for (size_t i = 0; i < exp.elems.size(); ++i)
	{
		if (exp[i].isOperator)
		{
			cmdWriter << "op " << GetOPString(exp[i].op);
		}
		else
		{
			if (exp[i].isConst)
			{
				cmdWriter << exp[i].constToken.strType << " " << exp[i].constToken.tokenString;
			}
			else
			{
				LogDescription(*(exp[i].desc));
			}
		}

		if (i < exp.elems.size() - 1)
		{
			cmdWriter << " ";
		}
	}
}

void CSemantics::CreateFunction()
{
	auto paramTable = static_cast<ParamTable*>(m_elems.top());
	m_elems.pop();
	for (size_t i = 0; i < paramTable->GetCount(); ++i)
	{
		auto elem = paramTable->GetElement(i);
		VarElement varElem;
		varElem.name = elem->GetName();
		varElem.hasFirstDim = false;
		varElem.hasSecondDim = false;
		varElem.scope = 1;
		varElem.type = elem->GetType();
		m_curVarTable.push_back(varElem);
	}

	m_stack.pop(); //miss both parenthesises
	m_stack.pop();

	std::string funcName = m_stack.top().token.tokenString;
	m_stack.pop();

	if (funcName == "main")
	{
		if (paramTable->GetCount() > 0)
		{
			m_error = true;
			std::cout << "Function \"main\" has parameters (expected 0)" << std::endl;
			return;
		}
		else
		{
			hasEntryPoint = true;
		}
	}

	auto funcType = m_stack.top().token.type;
	VarType varType;
	switch (funcType)
	{
	case TokenType::CHAR:
		varType = VarType::TYPE_CHAR;
		break;
	case TokenType::STR:
		varType = VarType::TYPE_STRING;
		break;
	case TokenType::FLOAT:
		varType = VarType::TYPE_FLOAT;
		break;
	case TokenType::INT:
		varType = VarType::TYPE_INT;
		break;
	case TokenType::VOID:
		varType = VarType::TYPE_VOID;
		break;
	default:
		assert(false);
		break;
	}
	m_stack.pop();

	FunctionTable* funcTable = new FunctionTable();
	funcTable->AddElement(new CFunction(funcName, varType, paramTable));
	m_funcTables.push_back(funcTable);

	cmdWriter << "func " << funcName << " ";
	LogVarType(varType);
	cmdWriter << paramTable->GetCount() << " ";
	for (size_t i = 0; i < paramTable->GetCount(); ++i)
	{
		auto elem = paramTable->GetElement(i);
		LogVarType(elem->GetType());
	}
	cmdWriter << std::endl;
}

void CSemantics::CreateParamList()
{
	std::stack<CParam*> tempStack;
	while (m_stack.top().label != Labels::FUNCTION_START_PARAM_LIST)
	{
		//paramTable->AddElement(static_cast<CParam*>(m_elems.top()));
		tempStack.push(static_cast<CParam*>(m_elems.top()));
		m_elems.pop();
		m_stack.pop();
	}
	m_stack.pop();

	ParamTable *paramTable = new ParamTable();
	while (!tempStack.empty())
	{
		auto elem = tempStack.top();
		paramTable->AddElement(std::move(elem));
		tempStack.pop();
	}

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
	case TokenType::CHAR:
		varType = VarType::TYPE_CHAR;
		break;
	case TokenType::STR:
		varType = VarType::TYPE_STRING;
		break;
	case TokenType::FLOAT:
		varType = VarType::TYPE_FLOAT;
		break;
	case TokenType::INT:
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
	for (size_t i = 0; i < m_curVarTable.size(); ++i)
	{
		if (m_curVarTable[i].name == varName)
		{
			return i;
		}
	}

	m_error = true;
	std::cout << "Error - unknown variable " << varName;
	return -1;
}

void CSemantics::RecognizeArrayPart()
{
	auto top = m_stack.top();
	bool hasFirst = false, hasSecond = false;
	ComplexExpression exp1, exp2;
	VarType firstType, secondType;
	if (top.label == Labels::START_ARRAY_SECOND_DIM)
	{
		hasSecond = true;
		exp2 = m_evalStack.top();
		m_evalStack.pop();
		m_stack.pop();
		secondType = m_types.top();
		m_types.pop();
	}

	m_stack.pop();
	hasFirst = true;
	exp1 = m_evalStack.top();
	m_evalStack.pop();
	firstType = m_types.top();
	m_types.pop();

	int pointer = GetVarInTable(m_stack.top().token.tokenString);
	if (m_error)
	{
		return;
	}

	if ((m_curVarTable[pointer].hasSecondDim && !hasSecond) || (!m_curVarTable[pointer].hasSecondDim && hasSecond) || 
		(!m_curVarTable[pointer].hasFirstDim && m_curVarTable[pointer].type != VarType::TYPE_STRING))
	{
		m_error = true;
		std::cout << "Dimensions don't match for variable " << m_stack.top().token.tokenString << std::endl;
		return;
	}

	if (m_curVarTable[pointer].firstDimType != firstType && m_curVarTable[pointer].type != VarType::TYPE_STRING)
	{
		m_error = true;
		std::cout << "Wrong type for first dimension " << m_stack.top().token.tokenString << std::endl;
		return;
	}

	if (hasSecond && m_curVarTable[pointer].secondDimType != secondType)
	{
		m_error = true;
		std::cout << "Wrong type for second dimension " << m_stack.top().token.tokenString << std::endl;
		return;
	}

	VariableDescription *desc = hasSecond ? new VariableDescription(pointer, exp1, exp2) : 
											new VariableDescription(pointer, exp1);
	m_varStack.push(desc);
	m_stack.pop();
	/*LogDescription(*desc);
	cmdWriter << std::endl;*/
}

void CSemantics::RecognizeFuncCall()
{
	m_stack.pop(); //first bracket
	FunctionCall *call = new FunctionCall();
	call->args = static_cast<FunctionArguments*>(m_elems.top());
	m_elems.pop();
	m_stack.pop(); //second bracket
	m_stack.pop(); //func_call_label
	auto top = m_stack.top();
	
	bool was = false;
	for (size_t i = 0; i < m_funcTables.size(); ++i)
	{
		if (m_funcTables[i]->GetElement(0)->GetName() == top.token.tokenString)
		{
			was = true;
			call->funcPointer = i;
			break;
		}
	}

	if (!was)
	{
		m_error = true;
		std::cout << "Function with name " << top.token.tokenString << " was not found" << std::endl;
		return;
	}

	for (size_t i = 0; i < m_funcTables[call->funcPointer]->GetCount(); ++i)
	{
		auto func = m_funcTables[call->funcPointer]->GetElement(i);
		auto params = func->GetParams();
		size_t paramCount = params->GetCount();
		if (paramCount != (*(call->args)).size())
		{
			m_error = true;
			std::cout << "Wrong argument count passed to " << top.token.tokenString << std::endl;
			return;
		}
		for (size_t j = 0; j < paramCount; ++j)
		{
			auto funcType = params->GetElement(j)->GetType();
			auto gotType = (*(call->args))[j].type;
			if (!(funcType == gotType || (funcType == VarType::TYPE_FLOAT && gotType == VarType::TYPE_INT)))
			{
				m_error = true;
				std::cout << "Wrong argument type at " << j + 1 << std::endl;
				return;
			}
		}
	}

	VariableDescription *desc = new VariableDescription();
	desc->func = call;
	desc->isFunctionCall = true;
	m_stack.pop();

	m_varStack.push(desc);
	//LogDescription(*desc);
	//cmdWriter << std::endl;
}

void CSemantics::RecognizeSimpleVar()
{
	auto top = m_stack.top();
	m_stack.pop();

	int pointer = GetVarInTable(top.token.tokenString);
	if (m_error)
	{
		return;
	}

	if (m_curVarTable[pointer].hasFirstDim)
	{
		m_error = true;
		std::cout << "Missing [ after variable " << top.token.tokenString << std::endl;
		return;
	}

	VariableDescription *desc = new VariableDescription(pointer);
	m_varStack.push(desc);
	//LogDescription(*desc);
	//cmdWriter << std::endl;
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
	int dim = 0;
	ComplexExpression dim1, dim2;
	if (!top.isToken)
	{
		if (top.label == Labels::START_ARRAY_SECOND_DIM)
		{
			dim++;
			newVarElement.hasSecondDim = true;
			dim2 = m_evalStack.top();
			m_evalStack.pop();
			auto curType = m_types.top();
			if (curType != VarType::TYPE_INT)
			{
				m_error = true;
				std::cout << "Wrong type dimension" << std::endl;
				return;
			}
			newVarElement.secondDimType = curType;
			m_types.pop();
			m_stack.pop();

			top = m_stack.top();
		}

		if (top.label == Labels::START_ARRAY_FIRST_DIM)
		{
			dim++;
			dim1 = m_evalStack.top();
			m_evalStack.pop();
			newVarElement.hasFirstDim = true;
			auto curType = m_types.top();
			if (curType != VarType::TYPE_INT)
			{
				m_error = true;
				std::cout << "Wrong type dimension" << std::endl;
				return;
			}
			newVarElement.firstDimType = curType;
			m_types.pop();
			m_stack.pop();

			top = m_stack.top();
		}
	}

	newVarElement.name = top.token.tokenString;
	for (size_t i = 0; i < m_curVarTable.size(); ++i)
	{
		if (m_curVarTable[i].name == newVarElement.name)
		{
			m_error = true;
			std::cout << "Already defined variable " << newVarElement.name << std::endl;
			return;
		}
	}
	newVarElement.scope = curScope;
	m_stack.pop();
	
	auto type = m_stack.top().token.type;
	VarType varType;
	switch (type)
	{
	case TokenType::CHAR:
		varType = VarType::TYPE_CHAR;
		break;
	case TokenType::STR:
		varType = VarType::TYPE_STRING;
		break;
	case TokenType::FLOAT:
		varType = VarType::TYPE_FLOAT;
		break;
	case TokenType::INT:
		varType = VarType::TYPE_INT;
		break;
	default:
		assert(false);
		break;
	}

	newVarElement.type = varType;
	
	m_varTable.push_back(newVarElement);
	m_curVarTable.push_back(newVarElement);

	cmdWriter << "new_var ";
	LogVarType(varType);
	cmdWriter << dim << " ";
	if (newVarElement.hasFirstDim)
	{
		cmdWriter << "fd ";
		LogExpression(dim1);
	}

	if (newVarElement.hasSecondDim)
	{
		cmdWriter << " sd ";
		LogExpression(dim2);
	}

	m_stack.pop();
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
	if (opString == "unary minus") return Operator::UNARY_MINUS;
	if (opString == "&&") return Operator::LOG_AND;
	if (opString == "||") return Operator::LOG_OR;
	if (opString == ">") return Operator::GREATER;
	if (opString == ">=") return Operator::GREATER_OR_EQUAL;
	if (opString == "<") return Operator::LESS;
	if (opString == "<=") return Operator::LESS_OR_EQUAL;
	if (opString == "==") return Operator::EQUAL;
	if (opString == "!=") return Operator::NOT_EQUAL;
	if (opString == "!") return Operator::NOT;
}

VarType CSemantics::GetType(TokenType type)
{
	switch (type)
	{
	case TokenType::CHARACTER:
		return VarType::TYPE_CHAR;
		break;
	case TokenType::STRING:
		return VarType::TYPE_STRING;
		break;
	case TokenType::FLOAT_NUMBER:
		return VarType::TYPE_FLOAT;
		break;
	case TokenType::INTEGER_DEC_NUMBER:
		return VarType::TYPE_INT;
		break;
	default:
		assert(false);
		break;
	}
}

bool CSemantics::FoundError()
{
	return m_error;
}

void CSemantics::CreateArithmeticExpression()
{
	ComplexExpression newExpr;
	std::stack<ComplexExpressionElement> reverseNotationStack;

	std::stack<StackType> tempStack;
	std::stack<VariableDescription*> stackForVariables;
	while (m_stack.top().label != Labels::START_ARITHMETIC_OPERATION)
	{
		tempStack.push(m_stack.top());
		if (m_stack.top().label == Labels::START_VAR_DESRIBE)
		{
			stackForVariables.push(m_varStack.top());
			m_varStack.pop();
		}
		m_stack.pop();
	}

	std::stack<VarType> curTypes;
	while (!tempStack.empty() && !m_error)
	{
		auto elem = tempStack.top();
		tempStack.pop();
		if (elem.isToken)
		{
			if (elem.token.type == TokenType::INTEGER_DEC_NUMBER || elem.token.type == TokenType::FLOAT_NUMBER ||
				elem.token.type == TokenType::STRING || elem.token.type == TokenType::CHARACTER)
			{
				newExpr.elems.push_back(elem.token);
				curTypes.push(GetType(elem.token.type));
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
							newExpr.elems.push_back(newElem);
							if (newElem.isConst)
							{
								curTypes.push(GetType(newElem.constToken.type));
							}
							else
							{
								if (newElem.isOperator)
								{
									if (newElem.op == Operator::UNARY_MINUS)
									{
										if (curTypes.top() != VarType::TYPE_FLOAT && curTypes.top() != VarType::TYPE_INT)
										{
											m_error = true;
											std::cout << "Unary minus ?" << newExpr.elems.back().constToken.tokenString;
											break;
										}
									}
									else
									{
										auto secArg = curTypes.top();
										curTypes.pop();
										auto firstArg = curTypes.top();
										curTypes.pop();

										if (!((secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_INT) ||
											(secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_INT)))
										{
											m_error = true;
											std::cout << "Wrong arithmetic expression" << std::endl;
											break;
										}
										else
										{
											VarType minType = VarType::TYPE_INT;
											if (firstArg == VarType::TYPE_FLOAT || secArg == VarType::TYPE_FLOAT)
											{
												minType = VarType::TYPE_FLOAT;
											}

											curTypes.push(minType);
										}
									}
								}
							}
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
								if (lastOP.op == Operator::UNARY_MINUS)
								{
									if (curTypes.top() != VarType::TYPE_FLOAT && curTypes.top() != VarType::TYPE_INT)
									{
										m_error = true;
										std::cout << "Unary minus ?" << newExpr.elems.back().constToken.tokenString;
										break;
									}
								}
								else
								{
									auto secArg = curTypes.top();
									curTypes.pop();
									auto firstArg = curTypes.top();
									curTypes.pop();

									if (!((secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_INT) ||
										(secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_INT)))
									{
										m_error = true;
										std::cout << "Wrong arithmetic expression" << std::endl;
										break;
									}
									else
									{
										VarType minType = VarType::TYPE_INT;
										if (firstArg == VarType::TYPE_FLOAT || secArg == VarType::TYPE_FLOAT)
										{
											minType = VarType::TYPE_FLOAT;
										}

										curTypes.push(minType);
									}
								}
								newExpr.elems.push_back(lastOP);
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
				auto varPoint = stackForVariables.top();
				stackForVariables.pop();
				newExpr.elems.push_back(varPoint);

				if (varPoint->isFunctionCall)
				{
					curTypes.push(m_funcTables[varPoint->func->funcPointer]->GetElement(0)->GetType());
				}
				else
				{
					curTypes.push(m_curVarTable[varPoint->pointer].type);
				}
			}
		}
	}

	if (m_error)
	{
		return;
	}

	while (!reverseNotationStack.empty() && !m_error)
	{
		auto newElem = reverseNotationStack.top();
		newExpr.elems.push_back(newElem);
		assert(newElem.isOperator);
		
		if (newElem.op == Operator::UNARY_MINUS)
		{
			if (curTypes.top() != VarType::TYPE_FLOAT && curTypes.top() != VarType::TYPE_INT)
			{
				m_error = true;
				std::cout << "Unary minus ?" << newExpr.elems.back().constToken.tokenString;
				break;
			}
		}
		else
		{
			auto secArg = curTypes.top();
			curTypes.pop();
			auto firstArg = curTypes.top();
			curTypes.pop();

			if (!((secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_INT) ||
				(secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_INT)))
			{
				m_error = true;
				std::cout << "Wrong arithmetic expression" << std::endl;
				break;
			}
			else
			{
				VarType minType = VarType::TYPE_INT;
				if (firstArg == VarType::TYPE_FLOAT || secArg == VarType::TYPE_FLOAT)
				{
					minType = VarType::TYPE_FLOAT;
				}

				curTypes.push(minType);
			}
		}

		reverseNotationStack.pop();
	}

	if (!m_error)
	{
		newExpr.type = curTypes.top();
		m_evalStack.push(newExpr);
		assert(curTypes.size() == 1);
		m_types.push(curTypes.top());
	}
}

void CSemantics::CreateConditionExpression()
{
	BooleanComplexExpression newExpr;
	std::stack<BooleanExpressionElement> reverseNotationStack;

	std::stack<StackType> tempStack;
	std::stack<ComplexExpression> stackForExpressions;
	while (m_stack.top().label != Labels::START_COND_EXPR)
	{
		tempStack.push(m_stack.top());
		if (m_stack.top().label == Labels::START_ARITHMETIC_OPERATION)
		{
			stackForExpressions.push(m_evalStack.top());
			m_evalStack.pop();
		}
		m_stack.pop();
	}

	std::stack<VarType> curTypes;
	while (!tempStack.empty() && !m_error)
	{
		auto elem = tempStack.top();
		tempStack.pop();
		if (elem.isToken)
		{
			if (elem.token.type == TokenType::INTEGER_DEC_NUMBER || elem.token.type == TokenType::FLOAT_NUMBER ||
				elem.token.type == TokenType::STRING || elem.token.type == TokenType::CHARACTER)
			{
				newExpr.push_back(elem.token);
				curTypes.push(GetType(elem.token.type));
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
						while ((reverseNotationStack.top().type != BooleanExpressionElement::ExpType::EXPRESSION_CONST) || 
								reverseNotationStack.top().constToken.tokenString != "(")
						{
							auto newElem = reverseNotationStack.top();
							newExpr.push_back(newElem);
							if (newElem.type == BooleanExpressionElement::ExpType::EXPRESSION_CONST)
							{
								curTypes.push(GetType(newElem.constToken.type));
							}
							else
							{
								if (newElem.type == BooleanExpressionElement::ExpType::EXPRESSION_OPERATOR)
								{
									if (newElem.op == Operator::NOT)
									{
										if (curTypes.top() != VarType::TYPE_BOOL)
										{
											m_error = true;
											std::cout << "Wrong condition in " << newExpr.back().constToken.tokenString;
											break;
										}
									}
									else
									{
										auto secArg = curTypes.top();
										curTypes.pop();
										auto firstArg = curTypes.top();
										curTypes.pop();

										if (newElem.op == Operator::LOG_AND || newElem.op == Operator::LOG_OR)
										{
											if (!(firstArg == VarType::TYPE_BOOL && secArg == VarType::TYPE_BOOL))
											{
												m_error = true;
												std::cout << "Wrong condition comparision" << std::endl;
												break;
											}
											else
											{
												VarType minType = VarType::TYPE_BOOL;
												curTypes.push(minType);
											}
										}
										else
										{
											if (!((secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_INT) ||
												(secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_INT)))
											{
												m_error = true;
												std::cout << "Wrong condition comparision" << std::endl;
												break;
											}
											else
											{
												VarType minType = VarType::TYPE_BOOL;
												curTypes.push(minType);
											}
										}
									}
								}
							}
							reverseNotationStack.pop();
						}
						reverseNotationStack.pop();
					}
					else //must be operator
					{
						auto opString = elem.token.tokenString;

						while (!reverseNotationStack.empty() && 
								reverseNotationStack.top().type == BooleanExpressionElement::ExpType::EXPRESSION_OPERATOR)
						{
							auto lastOP = reverseNotationStack.top();
							if (m_operationPrior.at(lastOP.op) >= m_operationPrior.at(GetOperatorByString(opString)))
							{
								if (lastOP.op == Operator::NOT)
								{
									if (curTypes.top() != VarType::TYPE_BOOL)
									{
										m_error = true;
										std::cout << "Wrong condition in " << newExpr.back().constToken.tokenString;
										break;
									}
								}
								else
								{
									auto secArg = curTypes.top();
									curTypes.pop();
									auto firstArg = curTypes.top();
									curTypes.pop();

									if (lastOP.op == Operator::LOG_AND || lastOP.op == Operator::LOG_OR)
									{
										if (!(firstArg == VarType::TYPE_BOOL && secArg == VarType::TYPE_BOOL))
										{
											m_error = true;
											std::cout << "Wrong condition comparision" << std::endl;
											break;
										}
										else
										{
											VarType minType = VarType::TYPE_BOOL;
											curTypes.push(minType);
										}
									}
									else
									{
										if (!((secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_INT) ||
											(secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_INT)))
										{
											m_error = true;
											std::cout << "Wrong condition comparision" << std::endl;
											break;
										}
										else
										{
											VarType minType = VarType::TYPE_BOOL;
											curTypes.push(minType);
										}
									}
								}
								newExpr.push_back(lastOP);
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
			if (elem.label == Labels::START_ARITHMETIC_OPERATION)
			{
				auto evalStack = stackForExpressions.top();
				newExpr.push_back(evalStack);
				stackForExpressions.pop();

				auto type = m_types.top();
				m_types.pop();

				curTypes.push(evalStack.type);
			}
		}
	}

	if (m_error)
	{
		return;
	}

	while (!reverseNotationStack.empty() && !m_error)
	{
		auto newElem = reverseNotationStack.top();
		newExpr.push_back(newElem);
		assert(newElem.type == BooleanExpressionElement::ExpType::EXPRESSION_OPERATOR);

		if (newElem.op == Operator::NOT)
		{
			if (curTypes.top() != VarType::TYPE_BOOL)
			{
				m_error = true;
				std::cout << "Wrong condition in " << newExpr.back().constToken.tokenString << " after NOT" << std::endl;
				break;
			}
		}
		else
		{
			auto secArg = curTypes.top();
			curTypes.pop();
			auto firstArg = curTypes.top();
			curTypes.pop();

			if (newElem.op == Operator::LOG_AND || newElem.op == Operator::LOG_OR)
			{
				if (!(firstArg == VarType::TYPE_BOOL && secArg == VarType::TYPE_BOOL))
				{
					m_error = true;
					std::cout << "Wrong condition comparision" << std::endl;
					break;
				}
				else
				{
					VarType minType = VarType::TYPE_BOOL;
					curTypes.push(minType);
				}
			}
			else
			{
				if (!((secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_FLOAT && firstArg == VarType::TYPE_INT) ||
					(secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_FLOAT) || (secArg == VarType::TYPE_INT && firstArg == VarType::TYPE_INT)))
				{
					m_error = true;
					std::cout << "Wrong condition comparision" << std::endl;
					break;
				}
				else
				{
					VarType minType = VarType::TYPE_BOOL;
					curTypes.push(minType);
				}
			}
		}

		reverseNotationStack.pop();
	}

	if (!m_error)
	{
		m_boolStack.push(newExpr);
		assert(curTypes.size() == 1);
		m_types.push(curTypes.top());
	}
}