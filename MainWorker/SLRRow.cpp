#include "stdafx.h"
#include "SLRRow.h"

CSLRRow::CSLRRow(std::map<std::string, Action> &&rowMap)
	:m_row(std::move(rowMap))
{
}

CSLRRow::Action CSLRRow::GetAction(std::string type) const
{
	if (m_row.count(type) == 0)
	{
		return Action(CSLRRow::Action::ActionType::REJECTED, (-1));
	}

	return m_row.at(type);
}