#pragma once
#include <vector>
#include <string>
#include "symbol_export.h"

class SYNTAX_DLL_API CSyntaxTable
{
public:
	typedef CSyntaxTable *Node;

	enum SYNTAX_ELEM_TYPE
	{
		NONE,
		ID,
		CONST, //добавить поддержку констант всех типов
		BIN_OPERATOR,
		COND_OPERATOR,
		TYPE,
		NONTERMINAL
	};

	CSyntaxTable(const std::string &name, SYNTAX_ELEM_TYPE type);
	~CSyntaxTable();

	std::vector<Node> GetAllChildren();
	void AddChild(Node child);
	void AddNext(Node next);

	std::string GetName() const;
	SYNTAX_ELEM_TYPE GetType() const;

private:
	Node m_child;
	Node m_next;

	std::string m_name;
	SYNTAX_ELEM_TYPE m_type;
};
