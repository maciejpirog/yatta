#ifndef WALKERS_H
#define WALKERS_H

#include "lexgen.h"
#include "parsegen.h"

#include "yg_lexer.h"
#include "yg_parser.h"
#include "yg_ast.h"

using namespace std;
using namespace yatta;

class lexer_input
{
public:
	static vector<string> token_names;
	static void lex(node& n, lexgen* lg);
protected:
	static token_type lextoken(node& n);
	static group lexgroup(node& n);
	static char get_char(string s);
	static atom lexatom(node& n, group* g);
};

class parser_input
{
public:
	static void parse(node& n, parsegen* pg);
protected:
	static rule parse_rule(node& n);
	static alternative parse_alternative(node& n);
	static parse_group parser_group(node& n);
	static parse_atom parser_atom(node& n);
	static vector<string> rule_names;
};	

#endif

