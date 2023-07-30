#ifndef AST_H
#define AST_H

#include <vector>
#include "yg_parser.h"

using namespace std;

namespace yatta {

enum rule_types {
	r_main,
	r_lexer,
	r_token,
	r_lexer_group,
	r_type,
	r_lexer_atom,
	r_parser,
	r_rule,
	r_alternative,
	r_parser_group,
	r_parser_atom
};

enum node_type { n_token, n_rule };

struct node
{
	node_type type;
	token t;
	rule_types rule;
	vector<node> children;

	node(node_type tp, token tt) : type(tp), t(tt) { }
	node() : type(n_token) { }
	void clear() { children.clear(); type = n_rule; }
};

}

#endif

