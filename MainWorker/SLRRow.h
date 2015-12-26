#pragma once
#include <map>
#include <vector>
#include <string>
#include "TokenType.h"

class CSLRRow
{
public:
	struct Action
	{
		enum ActionType
		{
			SHIFT,
			GOTO,
			REDUCE,
			ACCEPTED,
			REJECTED
		} action;

		size_t passTo;

		Action(ActionType act, size_t pass)
			:action(act), passTo(pass)
		{
		};
	};
	

	CSLRRow(std::map<std::string, Action> &&rowMap);

	Action GetAction(std::string type) const;

private:
	std::map<std::string, Action> m_row;
};

typedef std::vector<CSLRRow> SLRTable;