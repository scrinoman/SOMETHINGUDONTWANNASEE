#pragma once

#define MOVE_PARAM_DECL /*Means that value will be taken by class member from parameter*/

enum VarKind
{
	VAR,
	ARRAY,
	MAP_VAR
};

enum VarType
{
	TYPE_VOID,
	TYPE_INT,
	TYPE_CHAR,
	TYPE_FLOAT,
	TYPE_STRING
};

enum Labels
{
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
	START_ARRAY_FIRST_DIM,
	END_ARRAY_FIRST_DIM,
	START_ARRAY_SECOND_DIM,
	END_ARRAY_SECOND_DIM,
	START_MAP_DECL,
	END_MAP_DECL,
	START_FUNC_CALL,
	END_FUNC_CALL,
	START_CONDITION,
	END_CONDITION,
	START_FUNCTION_ARGUMENTS,
	END_FUNCTION_ARGUMENTS,
	START_FIRST_DIM,
	END_FIRST_DIM,
	START_SECOND_DIM,
	END_SECOND_DIM,
	LABEL_NONE,
	LABEL_START_PASS_FROM_NONTERMINAL
};