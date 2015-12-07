#pragma once
#include "SemanticTypes.h"
#include "SemanticTables.h"
#include <string>

class CFunction
{
public:
	CFunction(const std::string &name, VarType type, ParamTable MOVE_PARAM_DECL &params);
	
	std::string GetName() const;
	VarType GetType() const;
	ParamTable GetParams() const;

private:
	std::string m_name;
	VarType m_type;
	ParamTable m_params;
};

class FunctionTable : public SemanticTable<CFunction>
{
public:

	VarTable GetVarTable(size_t index) const
	{
		return m_varTables[index];
	}

	ActionTable GetActionTable(size_t index) const
	{
		return m_actionTables[index];
	}

	void AddVarTable(VarTable MOVE_PARAM_DECL &varTable)
	{
		m_varTables.push_back(std::move(varTable));
	}

	void AddActionTable(ActionTable MOVE_PARAM_DECL &actionTable)
	{
		m_actionTables.push_back(std::move(actionTable));
	}

private:
	std::vector<VarTable> m_varTables;
	std::vector<ActionTable> m_actionTables;
};