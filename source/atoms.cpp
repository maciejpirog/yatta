#include "lexer_atoms.h"
#include "parser_atoms.h"

token_type::token_type(string name, string text)
{
	this->name = name;
	skip = false;
	priv = false;

	group g;
	g.type = g_obligatory;
	atom a;
	a.type = a_string;
	a.token_name = text;
	g.atoms.push_back(a);
	groups.push_back(g);
}

token_type::token_type(string name, char ch)
{
	this->name = name;
	skip = false;
	priv = false;
	
	group g;
	g.type = g_obligatory;
	atom a;
	a.type = a_character;
	a.character = ch;
	g.atoms.push_back(a);
	groups.push_back(g);
}

token_type::token_type(string name, char ch1, char ch2, bool rev)
{
	this->name = name;
	skip = false;
	priv = false;
	
	if (ch1 > ch2) return;
	group g;
	g.type = g_obligatory;
	
	for (char i = ch1;  i<= ch2; i++)
	{
		atom a;
		a.type = a_character;
		a.character = i;
		a.reverse = rev;
		g.atoms.push_back(a);
	}
	groups.push_back(g);
}

void token_type::add_group(group& g)
{
	groups.push_back(g);
}

group::group(char ch)
{
	type = g_obligatory;
	atom a;
	a.type = a_character;
	a.character = ch;
	atoms.push_back(a);
}

group::group(string name)
{
	type = g_obligatory;
	atom a;
	a.type = a_token;;
	a.token_name = name;
	atoms.push_back(a);	
}
	
void group::add(char ch)
{
	atom a;
	a.type = a_character;
	a.character = ch;
	atoms.push_back(a);
}

void group::add(string name)
{
	atom a;
	a.type = a_token;
	a.token_name = name;
	atoms.push_back(a);	
}

alternative::alternative(parse_group pg)
{
	groups.push_back(pg);
}


