#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

namespace yatta {

class parser
{
protected:
	lexer l;

public:
	string filename;
	parser() : filename("line") { }
	node run(string data);
	inline bool match(int ptr, token_types type);
	static string translate_rules(rule_types r);
	int s_main(int i, node* n);
	int s_add_expr(int i, node* n);
	int s_mult_expr(int i, node* n);
	int s_base_expr(int i, node* n);

	// for error handling:
	token_types e_expected;
	token_types e_found;
	int e_line;
	string get_error();
};

}

#endif

