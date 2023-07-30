#include "walkers.h"
#include "exceptions.h"
#include <sstream>

using namespace std;

vector<string> lexer_input::token_names;

void lexer_input::lex(node& n, lexgen* lg)
{
	token_names.push_back("ENDOFFILE");
	for (size_t i = 0; i < n.children.size(); i++)
	{
		for (size_t j = 0; j < lexer_input::token_names.size(); j++)
			if (lexer_input::token_names[j] == n.children[i].children[1].t.text)
			{
				stringstream er;
				er << "error:" <<  n.children[i].children[1].t.line;
				er << ": token type '" << n.children[i].children[1].t.text << "' already defined";
				throw ex(er.str());
			}
		token_names.push_back(n.children[i].children[1].t.text);
	}
	for (size_t i = 0; i < n.children.size(); i++)
		lg->token_types.push_back(lextoken(n.children[i]));
}

token_type lexer_input::lextoken(node& n)
{
	token_type t;
	if (n.children[0].children[0].t.type == i_PRIV) t.priv = true;
	if (n.children[0].children[0].t.type == i_SKIP) t.skip = true;
	t.name = n.children[1].t.text;
	for (size_t i = 2; i < n.children.size(); i++)
		t.groups.push_back( lexgroup(n.children[i]) );
	return t;
}

group lexer_input::lexgroup(node& n)
{
	group g;
	for (size_t i = 0; i < n.children.size(); i++)
	{
		if (n.children[i].type == n_rule)
		{
			g.atoms.push_back( lexatom(n.children[i], &g) );
		}
		else
		{
			if (n.children[i].t.text == "?") g.type = g_optional;
			if (n.children[i].t.text == "*") g.type = g_repeat;
		}
	}
	return g;
}

char lexer_input::get_char(string s)
{
	if (s.size() < 3) throw ex("error");
	if (s.at(1) != '\\')
		return s.at(1);
	else
		if (s.at(2) == 'n') return '\n';
	else if (s.at(2) == 'r') return '\r';
	else if (s.at(2) == '\'') return '\'';
	else if (s.at(2) == '\"') return '\"';
	else if (s.at(2) == '\\') return '\\';
	else return ' ';
}

atom lexer_input::lexatom(node& n, group* g)
{
	atom a;
	size_t start = 0;
	if (n.children[start].t.type == i_REV)
	{
		a.reverse = true;
		start = 1;
	}
	if (n.children[start].t.type == i_ID)
	{
		bool tok = false;
		for (size_t i = 0; i < lexer_input::token_names.size(); i++)
		{
			if (lexer_input::token_names[i] == n.children[start].t.text)
			{
				tok = true;
				break;
			}
		}
		if (!tok)
		{
			stringstream er;
			er << "error:" <<  n.children[start].t.line;
			er << ": unknown identifier '" << n.children[start].t.text << "'";
			throw ex(er.str());
		}
		a.type = a_token;
		a.token_name = n.children[start].t.text;
	}
	if (n.children[start].t.type == i_STRING)
	{
		a.type = a_string;
		a.token_name = n.children[start].t.text.substr(1, n.children[start].t.text.size()-2);
	}
	if (n.children[start].t.type == i_LETTER)
	{
		if (n.children.size() >= start+2) // 'y'..'z'
		{
			char y = get_char(n.children[start].t.text);
			char z = get_char(n.children[start+1].t.text);
			if (y >= z)
			{
				stringstream er;
				er << "error:" <<  n.children[start].t.line;
				er << ": cannot resolve '" << y << "'..'" <<  z << "'";
				throw ex(er.str());
			}
			for (char i = y; i < z; i++)
			{
				atom k;
				k.type = a_character;
				k.character = i;
				g->atoms.push_back(k);
			}
			a.type = a_character;
			a.character = z;
		}
		else // 'a'
		{
			a.type = a_character;
			a.character = get_char(n.children[start].t.text);
		}
	}
	return a;
}


vector<string> parser_input::rule_names;

void parser_input::parse(node& n, parsegen* pg)
{
	for (size_t i = 0; i < n.children.size(); i++)
	{
		for (size_t j = 0; j < rule_names.size(); j++)
			if (rule_names[j] == n.children[i].children[0].t.text)
			{
				stringstream er;
				er << "error:" <<  n.children[i].children[0].t.line;
				er << ": rule '" << n.children[i].children[0].t.text << "' already defined";
				throw ex(er.str());
			}
		rule_names.push_back(n.children[i].children[0].t.text);
	}
	for (size_t i = 0; i < n.children.size(); i++)
		pg->rules.push_back(parse_rule(n.children[i]));
}

rule parser_input::parse_rule(node& n)
{
	rule r;
	r.name = n.children[0].t.text;
	for(size_t i = 1; i < n.children.size(); i++)
		r.alternatives.push_back(parse_alternative(n.children[i]));
	return r;
}

alternative parser_input::parse_alternative(node& n)
{
	alternative a;
	for(size_t i = 0; i < n.children.size(); i++)
		a.groups.push_back(parser_group(n.children[i]));
	return a;
}

parse_group parser_input::parser_group(node& n)
{
	parse_group g;
	for (size_t i = 0; i < n.children.size(); i++)
	{
		if (n.children[i].rule == r_parser_atom)
		{
			g.atoms.push_back( parser_atom(n.children[i]) );
		}
		else
		{
			if (n.children[i].t.text == "?") g.type = g_optional;
			if (n.children[i].t.text == "*") g.type = g_repeat;
		}
	}
	return g;
}

parse_atom parser_input::parser_atom(node& n)
{
	parse_atom a;
	size_t start = 0;
	if (n.children[start].t.text == "^")
	{
		a.root = true;
		start = 1;
	}
	else if (n.children[start].t.text == "!")
	{
		a.ignore = true;
		start = 1;
	}
	a.name = n.children[start].t.text;
	bool tok = false;
	for (size_t i = 0; i < lexer_input::token_names.size(); i++)
	{
		if (lexer_input::token_names[i] == n.children[start].t.text)
		{
			tok = true;
			break;
		}
	}
	if (tok)
		a.type = p_token;
	else
	{
		bool found = false;
		for (size_t i = 0; i < rule_names.size(); i++)
			if (rule_names[i] == n.children[start].t.text)
				found = true;
		if (!found)
		{
			stringstream er;
			er << "error:" <<  n.children[start].t.line;
			er << ": unknown identifier '" << n.children[start].t.text << "'";
			throw ex(er.str());
		}
		a.type = p_rule;
	}
	return a;
}
