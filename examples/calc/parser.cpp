#include <sstream>
#include "parser.h"

namespace yatta {

inline bool parser::match(int ptr, token_types type)
{
	if (l.tokens[ptr].type == type)
	{
		return true;
	}
	else
	{
		e_expected = type;
		e_found = l.tokens[ptr].type;
		e_line = l.tokens[ptr].line;
		return false;
	}
}

node parser::run(string data)
{
	l.filename = filename;
	l.run(data);
	node t;
	if(s_main(0, &t) < 0)
		throw exception(get_error(), e_line);
	else
	{
		t.rule = r_main;
		return t;
	}
}

string parser::get_error()
{
	std::stringstream str;
	str << filename << ":";
	str << e_line;
	str << ": expected token: ";
	str << l.translate_tokens(e_expected);
	str << " found: ";
	str << l.translate_tokens(e_found);
	str << "";
	return str.str();
}

string parser::translate_rules(rule_types r)
{
	if (r == r_main) return "main";
	if (r == r_add_expr) return "add_expr";
	if (r == r_mult_expr) return "mult_expr";
	if (r == r_base_expr) return "base_expr";
	return "";
}

int parser::s_main(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match add_expr
	{ node knn;
	if ((temp = s_add_expr(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_add_expr;
	nn.children.push_back(knn);
	ptr = temp; }

	// match token ENDOFFILE
	if (!match(ptr, ENDOFFILE)) { goto no_match_0; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

int parser::s_add_expr(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match mult_expr
	{ node knn;
	if ((temp = s_mult_expr(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_mult_expr;
	nn.children.push_back(knn);
	ptr = temp; }
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token add
		if (!match(lptr, add)) { break; } 
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		 lptr++;

		// match mult_expr
		{ node knn; if ((temp = s_mult_expr(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_mult_expr;
		lnn.children.push_back(knn);
		lptr = temp; }

		ptr = lptr;
		nn = lnn;
	}
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

int parser::s_mult_expr(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match base_expr
	{ node knn;
	if ((temp = s_base_expr(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_base_expr;
	nn.children.push_back(knn);
	ptr = temp; }
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token mult
		if (!match(lptr, mult)) { break; } 
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		 lptr++;

		// match base_expr
		{ node knn; if ((temp = s_base_expr(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_base_expr;
		lnn.children.push_back(knn);
		lptr = temp; }

		ptr = lptr;
		nn = lnn;
	}
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

int parser::s_base_expr(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match token lp
	if (!match(ptr, lp)) { goto no_match_0; } ptr++;

	// match add_expr
	{ node knn;
	if ((temp = s_add_expr(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_add_expr;
	nn.children.push_back(knn);
	ptr = temp; }

	// match token rp
	if (!match(ptr, rp)) { goto no_match_0; } ptr++;
	*n = nn;
	return ptr;
	no_match_0:

	// ----------------------
	ptr = i;
	nn.clear();

	// match token number
	if (!match(ptr, number)) { goto no_match_1; } 
	{ nn.t = l.tokens[ptr];
	nn.type = n_token; }
	ptr++;
	*n = nn;
	return ptr;
	no_match_1:

	return -1;
}

}

