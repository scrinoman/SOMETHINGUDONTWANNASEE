#pragma once
#include "stdafx.h"
#include "TokenType.h"

bool IsIntNumber(std::string const& s);
bool IsFloatNumber(std::string const& s);
bool IsChar(std::string const& s);
bool IsString(std::string const& s);
bool IsIdentifier(std::string const& s);