#include "stdafx.h"
#include "TokenAutomata.h"

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

	return false;
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
	
	if (!isdigit(s[i]) && s[i] != '.')
	{
		if ((s[i] == '-' && s.length() > 0) || (s[i] == '+' && s.length() > 0))
		{
			i++;
		}
		else
		{
			return false;
		}
	}
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
	if (s.length() > 4 || s.length() < 3)
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

bool IsIdentifier(std::string const& s)
{
	if (!isalpha(s[0]) && s[0] != '_') return false;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (!isalpha(s[i]) && !isdigit(s[i]) && s[i] != '_') return false;
	}
	return true;
}