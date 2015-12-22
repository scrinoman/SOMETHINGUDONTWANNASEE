#pragma once
#include "RuleTable.h"
#include <string>

#define CRT CRuleTable
#define CRR CRuleRow

using namespace std;

static const vector<CRuleTable> grammarTable = {
	CRT({}),
	CRT({ CRR("code", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(17) }, 2) }, LEFT_NONTERMINAL),
	CRT({ CRR("func", { TokenType(17), TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 6, Labels::FUNCTION_START, Labels::FUNCTION_END) }),
	CRT({ CRR("other_code", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(17) }, 4) }, LAST_NONTERMINAL),
	CRT({ CRR("other_code", { TokenType(17), TokenType(15), TokenType(10), TokenType(6), TokenType(12), }, 5) }, LEFT_NONTERMINAL),
	CRT({ CRR("code", { TokenType(17), TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 2) }, LAST_NONTERMINAL),
	CRT({ CRR("func", { TokenType(17), TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 7) }, LEFT_NONTERMINAL),
	CRT({ CRR("return_type", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(17) }, 15, Labels::FUNCTION_START_DECL) }),
	CRT({ CRR("id", { TokenType(1) }, 9) }, TERMINAL),
	CRT({ CRR("(", { TokenType(23) }, 10) }, TERMINAL),
	CRT({ CRR("idlist", { TokenType(24), TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 23, Labels::FUNCTION_START_PARAM_LIST, Labels::FUNCTION_END_PARAM_LIST) }),
	CRT({ CRR(")", { TokenType(24) }, 12, Labels::LABEL_NONE, Labels::FUNCTION_END_DECL) }, TERMINAL),
	CRT({ CRR("{", { TokenType(21) }, 13) }, TERMINAL),
	CRT({ CRR("explist", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9), TokenType(22) }, 35, Labels::START_EXPLIST, Labels::END_EXPLIST) }),
	CRT({ CRR("}", { TokenType(22) }, 0) }, TERMINAL),
	CRT({ CRR("return_type", { TokenType(17), }, 16), CRR("return_type", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 17) }, LEFT_NONTERMINAL),
	CRT({ CRR("void", { TokenType(17) }, 0) }, TERMINAL),
	CRT({ CRR("var_type", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 18) }, LAST_NONTERMINAL),
	CRT({ CRR("var_type", { TokenType(15) }, 19), CRR("var_type", { TokenType(10) }, 20), CRR("var_type", { TokenType(6) }, 21), CRR("var_type", { TokenType(12) }, 22) }, LEFT_NONTERMINAL),
	CRT({ CRR("int", { TokenType(15) }, 0) }, TERMINAL),
	CRT({ CRR("string", { TokenType(10) }, 0) }, TERMINAL),
	CRT({ CRR("char", { TokenType(6) }, 0) }, TERMINAL),
	CRT({ CRR("float", { TokenType(12) }, 0) }, TERMINAL),
	CRT({ CRR("idlist", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 24), CRR("idlist", { TokenType(24) }, 26) }, LEFT_NONTERMINAL),
	CRT({ CRR("id_decl", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 27) }),
	CRT({ CRR("other_decl", { TokenType(20), TokenType(24) }, 30) }, LAST_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(24) }, 0) }, EMPTY),
	CRT({ CRR("id_decl", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 28) }, LEFT_NONTERMINAL),
	CRT({ CRR("var_type", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 18, Labels::FUNCTION_START_PARAM) }),
	CRT({ CRR("id", { TokenType(1) }, 0, Labels::LABEL_NONE, Labels::FUNCTION_END_PARAM) }, TERMINAL),
	CRT({ CRR("other_decl", { TokenType(20) }, 31), CRR("other_decl", { TokenType(24) }, 34) }, LEFT_NONTERMINAL),
	CRT({ CRR(",", { TokenType(20) }, 32) }, TERMINAL),
	CRT({ CRR("id_decl", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 27) }),
	CRT({ CRR("other_decl", { TokenType(20), TokenType(24) }, 30) }, LAST_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(24) }, 0) }, EMPTY),
	CRT({ CRR("explist", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9) }, 36), CRR("explist", { TokenType(22) }, 38) }, LEFT_NONTERMINAL),
	CRT({ CRR("expression", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9) }, 39) }),
	CRT({ CRR("explist", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9), TokenType(22) }, 35) }, LAST_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(22) }, 0) }, EMPTY),
	CRT({ CRR("expression", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(9) }, 40), CRR("expression", { TokenType(1) }, 41), CRR("expression", { TokenType(7), TokenType(8) }, 42), CRR("expression", { TokenType(18), TokenType(13) }, 43), CRR("expression", { TokenType(14) }, 44), CRR("expression", { TokenType(16) }, 45) }, LEFT_NONTERMINAL),
	CRT({ CRR("decl_action", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(9) }, 46, Labels::START_DECL, Labels::END_DECL) }, LAST_NONTERMINAL),
	CRT({ CRR("op_action", { TokenType(1) }, 88, Labels::START_ASSIGN, Labels::END_ASSIGN) }, LAST_NONTERMINAL),
	CRT({ CRR("io_call", { TokenType(7), TokenType(8) }, 110) }, LAST_NONTERMINAL),
	CRT({ CRR("cycle", { TokenType(18), TokenType(13) }, 119) }, LAST_NONTERMINAL),
	CRT({ CRR("condition", { TokenType(14) }, 152, Labels::START_CONDITION, Labels::END_CONDITION) }, LAST_NONTERMINAL),
	CRT({ CRR("return_call", { TokenType(16) }, 167, Labels::START_RETURN, Labels::END_RETURN) }, LAST_NONTERMINAL),
	CRT({ CRR("decl_action", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 47), CRR("decl_action", { TokenType(9) }, 50) }, LEFT_NONTERMINAL),
	CRT({ CRR("var_decl", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 52, Labels::START_VAR_DECL, Labels::END_VAR_DECL) }),
	CRT({ CRR("decl_right", { TokenType(39), TokenType(19) }, 75) }),
	CRT({ CRR(";", { TokenType(19) }, 0) }, TERMINAL),
	CRT({ CRR("map_decl", { TokenType(9) }, 56, Labels::START_MAP_DECL, Labels::END_MAP_DECL) }),
	CRT({ CRR(";", { TokenType(19) }, 0) }, TERMINAL),
	CRT({ CRR("var_decl", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 53) }, LEFT_NONTERMINAL),
	CRT({ CRR("var_type", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 18) }),
	CRT({ CRR("id", { TokenType(1) }, 55) }, TERMINAL),
	CRT({ CRR("array_part", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32), TokenType(25) }, 64) }, LAST_NONTERMINAL),
	CRT({ CRR("map_decl", { TokenType(9) }, 57) }, LEFT_NONTERMINAL),
	CRT({ CRR("map", { TokenType(9) }, 58) }, TERMINAL),
	CRT({ CRR("<", { TokenType(34) }, 59) }, TERMINAL),
	CRT({ CRR("var_type", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 18) }),
	CRT({ CRR(",", { TokenType(20) }, 61) }, TERMINAL),
	CRT({ CRR("var_type", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 18) }),
	CRT({ CRR(">", { TokenType(35) }, 63) }, TERMINAL),
	CRT({ CRR("id", { TokenType(1) }, 0) }, TERMINAL),
	CRT({ CRR("array_part", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32) }, 65), CRR("array_part", { TokenType(25) }, 66) }, LEFT_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32) }, 0) }, EMPTY),
	CRT({ CRR("[", { TokenType(25) }, 67, Labels::START_ARRAY_FIRST_DIM) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(29), TokenType(1), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(23) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR("]", { TokenType(26) }, 69, Labels::LABEL_NONE, Labels::END_ARRAY_FIRST_DIM) }, TERMINAL),
	CRT({ CRR("second_dimension", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32), TokenType(25) }, 70) }, LAST_NONTERMINAL),
	CRT({ CRR("second_dimension", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32) }, 71), CRR("second_dimension", { TokenType(25) }, 72) }, LEFT_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32) }, 0) }, EMPTY),
	CRT({ CRR("[", { TokenType(25) }, 73, Labels::START_ARRAY_SECOND_DIM) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(29), TokenType(1), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(23) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR("]", { TokenType(26) }, 0, Labels::LABEL_NONE, Labels::END_ARRAY_SECOND_DIM) }, TERMINAL),
	CRT({ CRR("decl_right", { TokenType(39) }, 76), CRR("decl_right", { TokenType(19) }, 78) }, LEFT_NONTERMINAL),
	CRT({ CRR("=", { TokenType(39) }, 77) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(29) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING_AND_LAST_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(19) }, 0) }, EMPTY),
	CRT({ CRR("low_prior_bin_op", { TokenType(28) }, 80), CRR("low_prior_bin_op", { TokenType(29) }, 81), CRR("low_prior_bin_op", { TokenType(43) }, 82), CRR("low_prior_bin_op", { TokenType(32) }, 83) }, LEFT_NONTERMINAL),
	CRT({ CRR("+", { TokenType(28) }, 0) }, TERMINAL),
	CRT({ CRR("-", { TokenType(29) }, 0) }, TERMINAL),
	CRT({ CRR("|", { TokenType(43) }, 0) }, TERMINAL),
	CRT({ CRR("%", { TokenType(32) }, 0) }, TERMINAL),
	CRT({ CRR("high_prior_bin_op", { TokenType(30) }, 85), CRR("high_prior_bin_op", { TokenType(31) }, 86), CRR("high_prior_bin_op", { TokenType(42) }, 87) }, LEFT_NONTERMINAL),
	CRT({ CRR("*", { TokenType(30) }, 0) }, TERMINAL),
	CRT({ CRR("/", { TokenType(31) }, 0) }, TERMINAL),
	CRT({ CRR("&", { TokenType(42) }, 0) }, TERMINAL),
	CRT({ CRR("op_action", { TokenType(1) }, 89) }, LEFT_NONTERMINAL),
	CRT({ CRR("left_part_wo_functions", { TokenType(1) }, 171) }),
	CRT({ CRR("=", { TokenType(39) }, 91) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(29) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR(";", { TokenType(19) }, 0) }, TERMINAL),
	CRT({ CRR("left_part", { TokenType(1) }, 94) }, LEFT_NONTERMINAL),
	CRT({ CRR("id", { TokenType(1) }, 95, Labels::START_VAR_DESRIBE) }, TERMINAL),
	CRT({ CRR("next_part", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32), TokenType(25), TokenType(23) }, 96, Labels::LABEL_NONE, Labels::END_VAR_DESCRIBE) }, LAST_NONTERMINAL),
	CRT({ CRR("next_part", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32), TokenType(25) }, 97), CRR("next_part", { TokenType(23) }, 98) }, LEFT_NONTERMINAL),
	CRT({ CRR("array_part", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32), TokenType(25) }, 64) }, LAST_NONTERMINAL),
	CRT({ CRR("(", { TokenType(23) }, 99, Labels::START_FUNC_CALL) }, TERMINAL),
	CRT({ CRR("func_list", { TokenType(29), TokenType(1), TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(24) }, 101, Labels::START_FUNCTION_ARGUMENTS, Labels::END_FUNCTION_ARGUMENTS) }),
	CRT({ CRR(")", { TokenType(24) }, 0, Labels::LABEL_NONE, Labels::END_FUNC_CALL) }, TERMINAL),
	CRT({ CRR("func_list", { TokenType(29), TokenType(1), TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5) }, 103, Labels::START_FUNCTION_ARG), CRR("func_list", { TokenType(24) }, 102) }, LEFT_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(24) }, 0) }, EMPTY),
	CRT({ CRR("right_part", { TokenType(29), TokenType(1), TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR("other_func_list", { TokenType(24), TokenType(20) }, 105, Labels::END_FUNCTION_ARG) }, LAST_NONTERMINAL),
	CRT({ CRR("other_func_list", { TokenType(24) }, 106), CRR("other_func_list", { TokenType(20) }, 107) }, LEFT_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(24) }, 0) }, EMPTY),
	CRT({ CRR(",", { TokenType(20) }, 108, Labels::LABEL_NONE, Labels::START_FUNCTION_ARG) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(29), TokenType(1), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(23) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR("other_func_list", { TokenType(24), TokenType(20) }, 105, Labels::END_FUNCTION_ARG) }, LAST_NONTERMINAL),
	CRT({ CRR("io_call", { TokenType(7) }, 111), CRR("io_call", { TokenType(8) }, 115) }, LEFT_NONTERMINAL),
	CRT({ CRR("cin", { TokenType(7) }, 112, Labels::START_INPUT) }, TERMINAL),
	CRT({ CRR(">>", { TokenType(48) }, 113) }, TERMINAL),
	CRT({ CRR("left_part_wo_functions", { TokenType(1) }, 171) }),
	CRT({ CRR(";", { TokenType(19) }, 0, Labels::LABEL_NONE, Labels::END_INPUT) }, TERMINAL),
	CRT({ CRR("cout", { TokenType(8) }, 116, Labels::START_OUTPUT) }, TERMINAL),
	CRT({ CRR("<<", { TokenType(47) }, 117) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(29) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR(";", { TokenType(19) }, 0, Labels::LABEL_NONE, Labels::END_OUTPUT) }, TERMINAL),
	CRT({ CRR("cycle", { TokenType(18) }, 120), CRR("cycle", { TokenType(13) }, 127) }, LEFT_NONTERMINAL),
	CRT({ CRR("while", { TokenType(18) }, 121, Labels::START_WHILE) }, TERMINAL),
	CRT({ CRR("(", { TokenType(23) }, 122) }, TERMINAL),
	CRT({ CRR("cond_expr", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(45) }, 0, Labels::START_COND_EXPR, Labels::END_COND_EXPR) }, NEED_SLR_PARSING),
	CRT({ CRR(")", { TokenType(24) }, 124) }, TERMINAL),
	CRT({ CRR("{", { TokenType(21) }, 125) }, TERMINAL),
	CRT({ CRR("explist", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9), TokenType(22) }, 35, Labels::START_EXPLIST, Labels::END_EXPLIST) }),
	CRT({ CRR("}", { TokenType(22) }, 0, Labels::LABEL_NONE, Labels::END_WHILE) }, TERMINAL),
	CRT({ CRR("for", { TokenType(13) }, 128, Labels::START_FOR) }, TERMINAL),
	CRT({ CRR("(", { TokenType(23) }, 129) }, TERMINAL),
	CRT({ CRR("var_decl", { TokenType(15), TokenType(10), TokenType(6), TokenType(12) }, 52) }),
	CRT({ CRR("=", { TokenType(39) }, 131) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(29) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR(";", { TokenType(19) }, 133) }, TERMINAL),
	CRT({ CRR("cond_expr", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(45) }, 0, Labels::START_COND_EXPR, Labels::END_COND_EXPR) }, NEED_SLR_PARSING),
	CRT({ CRR(";", { TokenType(19) }, 135) }, TERMINAL),
	CRT({ CRR("id", { TokenType(1) }, 136) }, TERMINAL),
	CRT({ CRR("=", { TokenType(39) }, 137) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(29) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR(")", { TokenType(24) }, 139) }, TERMINAL),
	CRT({ CRR("{", { TokenType(21) }, 140) }, TERMINAL),
	CRT({ CRR("explist", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9), TokenType(22) }, 35, Labels::START_EXPLIST, Labels::END_EXPLIST) }),
	CRT({ CRR("}", { TokenType(22) }, 0, Labels::END_FOR) }, TERMINAL),
	CRT({ CRR("bool_op", { TokenType(40) }, 143), CRR("bool_op", { TokenType(41) }, 144) }, LEFT_NONTERMINAL),
	CRT({ CRR("&&", { TokenType(40) }, 0) }, TERMINAL),
	CRT({ CRR("||", { TokenType(41) }, 0) }, TERMINAL),
	CRT({ CRR("rel_op", { TokenType(35) }, 146), CRR("rel_op", { TokenType(34) }, 147), CRR("rel_op", { TokenType(46) }, 148), CRR("rel_op", { TokenType(38) }, 149), CRR("rel_op", { TokenType(36) }, 150), CRR("rel_op", { TokenType(37) }, 151) }, LEFT_NONTERMINAL),
	CRT({ CRR(">", { TokenType(35) }, 0) }, TERMINAL),
	CRT({ CRR("<", { TokenType(34) }, 0) }, TERMINAL),
	CRT({ CRR("!=", { TokenType(46) }, 0) }, TERMINAL),
	CRT({ CRR("==", { TokenType(38) }, 0) }, TERMINAL),
	CRT({ CRR("<=", { TokenType(36) }, 0) }, TERMINAL),
	CRT({ CRR(">=", { TokenType(37) }, 0) }, TERMINAL),
	CRT({ CRR("condition", { TokenType(14) }, 153) }, LEFT_NONTERMINAL),
	CRT({ CRR("if", { TokenType(14) }, 154, Labels::START_IF) }, TERMINAL),
	CRT({ CRR("(", { TokenType(23) }, 155) }, TERMINAL),
	CRT({ CRR("cond_expr", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(45) }, 0, Labels::START_COND_EXPR, Labels::END_COND_EXPR) }, NEED_SLR_PARSING),
	CRT({ CRR(")", { TokenType(24) }, 157) }, TERMINAL),
	CRT({ CRR("{", { TokenType(21) }, 158) }, TERMINAL),
	CRT({ CRR("explist", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9), TokenType(22) }, 35, Labels::START_EXPLIST, Labels::END_EXPLIST) }),
	CRT({ CRR("}", { TokenType(22) }, 0, Labels::LABEL_NONE, Labels::END_IF) }, TERMINAL),
	CRT({ CRR("else", { TokenType(22), TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9), TokenType(11) }, 161) }, LAST_NONTERMINAL),
	CRT({ CRR("else", { TokenType(22), TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9) }, 162), CRR("else", { TokenType(11) }, 163) }, LEFT_NONTERMINAL),
	CRT({ CRR("empty", { TokenType(22), TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9), }, 0) }, EMPTY),
	CRT({ CRR("else", { TokenType(11) }, 164, Labels::START_ELSE) }, TERMINAL),
	CRT({ CRR("{", { TokenType(21) }, 165) }, TERMINAL),
	CRT({ CRR("explist", { TokenType(15), TokenType(10), TokenType(6), TokenType(12), TokenType(1), TokenType(7), TokenType(8), TokenType(18), TokenType(14), TokenType(13), TokenType(16), TokenType(9), TokenType(22) }, 35, Labels::START_EXPLIST, Labels::END_EXPLIST) }),
	CRT({ CRR("}", { TokenType(22) }, 0, Labels::LABEL_NONE, Labels::END_ELSE) }, TERMINAL),
	CRT({ CRR("return_call", { TokenType(16) }, 168) }, LEFT_NONTERMINAL),
	CRT({ CRR("return", { TokenType(16) }, 169) }, TERMINAL),
	CRT({ CRR("right_part", { TokenType(23), TokenType(2), TokenType(3), TokenType(4), TokenType(5), TokenType(1), TokenType(29) }, 0, Labels::START_ARITHMETIC_OPERATION, Labels::END_ARITHMETIC_OPERATION) }, NEED_SLR_PARSING),
	CRT({ CRR(";", { TokenType(19) }, 0) }, TERMINAL),
	CRT({ CRR("left_part_wo_functions", { TokenType(1) }, 172) }, LEFT_NONTERMINAL),
	CRT({ CRR("id", { TokenType(1) }, 173, Labels::START_VAR_DESRIBE) }, TERMINAL),
	CRT({ CRR("array_part", { TokenType(39), TokenType(19), TokenType(30), TokenType(31), TokenType(42), TokenType(28), TokenType(29), TokenType(43), TokenType(26), TokenType(24), TokenType(20), TokenType(38), TokenType(46), TokenType(35), TokenType(34), TokenType(37), TokenType(36), TokenType(40), TokenType(41), TokenType(32), TokenType(25) }, 64, Labels::LABEL_NONE, Labels::END_VAR_DESCRIBE) }, LAST_NONTERMINAL)
};