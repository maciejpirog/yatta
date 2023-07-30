#ifndef PARSER_ATOMS_H
#define PARSER_ATOMS_H

#include <string>
#include <vector>

#include "lexer_atoms.h"

using namespace std;

//from lexer_atoms.h : enum group_type: g_obligatory, g_optional, g_repeat
enum parse_atom_type { p_token, p_rule };

struct parse_atom
{
	parse_atom_type type;
	string name;
	
	// for AST construction:
	bool ignore;
	bool root;
	
	parse_atom() : ignore(false), root(false) { }
	parse_atom(parse_atom_type t, string s) : type(t), name(s), ignore(false), root(false) { }
	parse_atom(parse_atom_type t, string s, bool ignore) : type(t), name(s), ignore(ignore), root(false) { }
};

struct parse_group
{
	group_type type;
	vector<parse_atom> atoms;
	
	parse_group() : type(g_obligatory) { }
};

struct alternative
{
	vector<parse_group> groups;
	
	alternative() { }
	alternative(parse_group pg);
};

struct rule
{
	string name;
	vector<alternative> alternatives;
	
	rule() { }
	rule(string n) : name(n) { }
};

#endif


