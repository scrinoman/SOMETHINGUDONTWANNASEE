#pragma once

#include "Lexer.h"
#include "TokenType.h"

#define MOVE_PARAM_DECL /*Means that value might be taken by class member from parameter*/

enum struct VarKind
{
	VAR,
	ARRAY,
	MAP_VAR
};

enum struct VarType
{
	TYPE_VOID,
	TYPE_INT,
	TYPE_CHAR,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_BOOL //HIDDEN TYPE
};

enum Labels
{
	START_CODE,
	END_CODE,
	FUNCTION_START,
	FUNCTION_END,
	FUNCTION_START_DECL,
	FUNCTION_END_DECL,
	FUNCTION_START_PARAM_LIST,
	FUNCTION_END_PARAM_LIST,
	FUNCTION_START_PARAM,
	FUNCTION_END_PARAM,
	START_EXPLIST,
	END_EXPLIST,
	START_DECL,
	END_DECL,
	START_ASSIGN,
	END_ASSIGN,
	START_INPUT,
	END_INPUT,
	START_OUTPUT,
	END_OUTPUT,
	START_WHILE,
	END_WHILE,
	START_FOR,
	END_FOR,
	START_CONDITION,
	END_CONDITION,
	START_IF,
	END_IF,
	START_ELSE,
	END_ELSE,
	START_RETURN,
	END_RETURN,
	START_VAR_DECL,
	END_VAR_DECL,
	START_ARITHMETIC_OPERATION,
	END_ARITHMETIC_OPERATION,
	START_COND_EXPR,
	END_COND_EXPR,
	START_ARRAY_FIRST_DIM,
	END_ARRAY_FIRST_DIM,
	START_ARRAY_SECOND_DIM,
	END_ARRAY_SECOND_DIM,
	START_MAP_DECL,
	END_MAP_DECL,
	START_FUNC_CALL,
	END_FUNC_CALL,
	START_FUNCTION_ARGUMENTS,
	END_FUNCTION_ARGUMENTS,
	START_FUNCTION_ARG,
	END_FUNCTION_ARG,
	START_FIRST_DIM,
	END_FIRST_DIM,
	START_SECOND_DIM,
	END_SECOND_DIM,
	START_VAR_DESRIBE,
	END_VAR_DESCRIBE,
	START_VAR_DECL_RIGHT,
	END_VAR_DECL_RIGHT,
	START_IF_DECL,
	END_IF_DECL,
	START_WHILE_COND,
	END_WHILE_COND,
	START_FOR_VAR_DESC,
	END_FOR_VAR_DESC,
	START_FOR_VAR_EXPR,
	END_FOR_VAR_EXPR,
	START_FOR_COND,
	END_FOR_COND,
	START_FOR_CHANGE,
	END_FOR_CHANGE,
	LABEL_NONE
};

enum struct Operator
{
	PLUS,
	MINUS,
	OR,
	MOD,
	MULT,
	DIV,
	UNARY_MINUS,
	AND,
	LOG_AND,
	LOG_OR,
	GREATER,
	LESS,
	EQUAL,
	NOT_EQUAL,
	GREATER_OR_EQUAL,
	LESS_OR_EQUAL,
	NOT
};

struct VarElement
{
	bool isMap = false;
	std::string name = "";
	VarType type = VarType::TYPE_VOID;
	int scope = -1;

	bool hasFirstDim = false, hasSecondDim = false;
	VarType firstDimType, secondDimType;
};

struct VariableDescription;
struct ComplexExpressionElement
{
	bool isOperator = false;
	bool isConst = false;
	Operator op = Operator::PLUS;
	Token constToken;
	VariableDescription *desc = nullptr;
	VarType returnType = VarType::TYPE_VOID;

	ComplexExpressionElement()
	{
	}

	ComplexExpressionElement(Operator newOperator)
		: op(newOperator), isOperator(true), isConst(false)
	{
	}

	ComplexExpressionElement(const Token &token)
		: constToken(token), isOperator(false), isConst(true)
	{
	}

	ComplexExpressionElement(VariableDescription *newDesc)
		: isOperator(false), isConst(false), desc(newDesc)
	{
	}
};

struct ComplexExpression
{
	std::vector<ComplexExpressionElement> elems;
	VarType type;

	ComplexExpressionElement operator[](size_t index) const
	{
		return elems[index];
	}
};

struct BooleanExpressionElement
{
	enum struct ExpType
	{
		EXPRESSION_NONE,
		EXPRESSION_OPERATOR,
		EXPRESSION_CONST,
		EXPRESSION_COMPLEX
	} type = ExpType::EXPRESSION_NONE;

	Operator op = Operator::PLUS;
	Token constToken;
	ComplexExpression elem;

	BooleanExpressionElement()
	{
	}

	BooleanExpressionElement(Operator newOperator)
		: op(newOperator), type(ExpType::EXPRESSION_OPERATOR)
	{
	}

	BooleanExpressionElement(const Token &token)
		: constToken(token), type(ExpType::EXPRESSION_CONST)
	{
	}

	BooleanExpressionElement(const ComplexExpression &newExp)
		: elem(newExp), type(ExpType::EXPRESSION_COMPLEX)
	{
	}
};

typedef std::vector<BooleanExpressionElement> BooleanComplexExpression;

struct FunctionArgument
{
	ComplexExpression exp;
	VarType type;

	FunctionArgument(const ComplexExpression &expression, VarType newType)
		:exp(expression), type(newType)
	{
	}
};

typedef std::vector<FunctionArgument> FunctionArguments;

struct FunctionCall
{
	int funcPointer = -1;
	FunctionArguments *args = nullptr;
};

struct VariableDescription
{
	bool isFunctionCall = false;
	FunctionCall *func = nullptr;

	int pointer = -1;

	bool isStringDereference = false;
	bool hasFirstDim = false, hasSecondDim = false;
	ComplexExpression firstDim, secondDim;

	VariableDescription()
	{
	}

	VariableDescription(int newPointer)
		:pointer(newPointer), hasFirstDim(false), hasSecondDim(false)
	{
	}

	VariableDescription(int newPointer, const ComplexExpression &newComplexPart)
		:pointer(newPointer), firstDim(newComplexPart), hasFirstDim(true), hasSecondDim(false)
	{
	}

	VariableDescription(int newPointer, const ComplexExpression &newFirstComplexPart, const ComplexExpression &newSecondComplexPart)
		:pointer(newPointer), firstDim(newFirstComplexPart), secondDim(newSecondComplexPart), hasFirstDim(true), hasSecondDim(true)
	{
	}
};