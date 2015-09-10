#pragma once
#include "stdafx.h"
#include "TokenAutomata.h"
#include "TokenType.h"

std::map<TokenType, std::function<bool(const std::string&)>> tokenFunc =
{
	{ TokenType::INTEGER_DEC_NUMBER, IsIntNumber },
	{ TokenType::FLOAT_NUMBER, IsFloatNumber },
	{ TokenType::CHARACTER, IsChar },
	{ TokenType::STRING, IsString },
	{ TokenType::INTEGER_OCT_NUMBER, IsOCT },
	{ TokenType::INTEGER_HEX_NUMBER, IsHEX },
	{ TokenType::IDENTIFIER, IsIdentifier}
};