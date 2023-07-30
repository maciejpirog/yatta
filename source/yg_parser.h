#ifndef PARSER_H
#define PARSER_H

#include "yg_lexer.h"
#include "yg_ast.h"

namespace yatta {

class parser
{
protected:
	lexer l;

public:
	string filename;
	parser() : filename("line") { }
	node run(string data);

protected:
	inline bool match(int ptr, token_types type);
	static string translate_rules(rule_types r);
	int s_main(int i, node* n);
	int s_lexer(int i, node* n);
	int s_token(int i, node* n);
	int s_lexer_group(int i, node* n);
	int s_type(int i, node* n);
	int s_lexer_atom(int i, node* n);
	int s_parser(int i, node* n);
	int s_rule(int i, node* n);
	int s_alternative(int i, node* n);
	int s_parser_group(int i, node* n);
	int s_parser_atom(int i, node* n);

public:
	// for error handling:
	token_types e_expected;
	token_types e_found;
	int e_line;
	string get_error();
};

}

#endif

