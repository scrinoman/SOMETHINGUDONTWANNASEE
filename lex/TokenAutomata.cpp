#include "stdafx.h"
#include "TokenAutomata.h"

bool IsValidIntModifier(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	return (s == "u" || s == "l" || s == "lu" || s == "ul" || s == "ll" || s == "ull" || s == "lul" || s == "llu");
}

bool IsIntNumber(const std::string &s)
{
	size_t i = 0;
	if (!isdigit(s[0]))
		return false;
	while (i < s.length() && isdigit(s[i]))
	{
		i++;
	}
	if (i == s.length())
	{
		return true;
	}
	std::string end = s.substr(i, s.length() - i);
	return IsValidIntModifier(end);
}

bool IsValidEpsNotation(std::string const& s, size_t & i)
{
	if (s[i] == 'e')
	{
		++i;
		if (s[i] != '+' && s[i] != '-' && !isdigit(s[i]))
		{
			return false;
		}
		if (s[i] == '+' || s[i] == '-')
		{
			++i;
		}
		while (i < s.length() && isdigit(s[i]))
		{
			i++;
		}
		if (i < s.length())
		{
			return ::tolower(s[i]) == 'l' && i == s.length() - 1;
		}
		return true;
	}
	if (i < s.length())
	{
		return ::tolower(s[i]) == 'l' && i == s.length() - 1;
	}
	return true;
}

bool IsFloatNumber(std::string const& s)
{
	size_t i = 0;
	if (!isdigit(s[0]) && s[0] != '.')
		return false;
	while (i < s.length() && isdigit(s[i]))
	{
		i++;
	}
	if (s[i] == '.')
	{
		++i;
		while (i < s.length() && isdigit(s[i]))
		{
			i++;
		}
		return IsValidEpsNotation(s, i);
	}
	else if (s.length() == 1 && isdigit(s[0]))
	{
		return false;
	}
	return IsValidEpsNotation(s, i);

}

bool IsChar(std::string const& s)
{
	if (s[0] != '\'')
	{
		return false;
	}
	if (s.length() > 5 || s.length() < 3)
	{
		return false;
	}
	if (s.length() == 4 && s[1] != '\\')
	{
		return false;
	}
	if (s[s.length() - 1] != '\'')
	{
		return false;
	}
	return true;
}

bool IsString(std::string const& s)
{
	if (s[0] != '\"')
	{
		return false;
	}
	size_t i = 1;
	bool flag = false;
	while (i < s.size() - 1)
	{
		if (s[i] == '\"' && s[i - 1] != '\\')
		{
			if (flag)
			{
				flag = false;
			}
			else
			{
				flag = true;
			}
		}
		else
		{
			if (flag)
			{
				return false;
			}
		}
		i++;
	}
	if (!flag)	return s[s.size() - 1] == '\"';
	return false;
}

bool IsHEX(std::string s)
{
	if (s.length() < 3) return false;
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	if (s[0] != '0' || s[1] != 'x') return false;
	std::set<char> hexers = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	for (size_t i = 2; i < s.length(); ++i)
	{
		if (!hexers.count(s[i])) return false;
	}
	return true;
}

bool IsOCT(const std::string &s)
{
	if (s.length() < 2) return false;
	if (s[0] != '0') return false;
	std::set<char> octers = { '0', '1', '2', '3', '4', '5', '6', '7'};
	for (size_t i = 1; i < s.length(); ++i)
	{
		if (!octers.count(s[i])) return false;
	}
	return true;
}

bool IsIdentifier(std::string const& s)
{
	if (!isalpha(s[0]) && s[0] != '_') return false;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (!isalpha(s[i]) && !isdigit(s[i]) && s[i] != '_') return false;
	}
	return true;
}