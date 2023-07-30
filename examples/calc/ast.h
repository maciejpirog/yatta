#ifndef AST_H
#define AST_H

#include <vector>
#include "parser.h"

using namespace std;

namespace yatta {

enum rule_types {
	r_main,
	r_add_expr,
	r_mult_expr,
	r_base_expr
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

