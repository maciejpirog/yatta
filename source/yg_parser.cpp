#include <sstream>
#include "yg_parser.h"

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
		return t;
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
	if (r == r_lexer) return "lexer";
	if (r == r_token) return "token";
	if (r == r_lexer_group) return "lexer_group";
	if (r == r_type) return "type";
	if (r == r_lexer_atom) return "lexer_atom";
	if (r == r_parser) return "parser";
	if (r == r_rule) return "rule";
	if (r == r_alternative) return "alternative";
	if (r == r_parser_group) return "parser_group";
	if (r == r_parser_atom) return "parser_atom";
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

	// match token LEXER
	if (!match(ptr, i_LEXER)) { goto no_match_0; } ptr++;

	// match lexer
	{ node knn;
	if ((temp = s_lexer(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_lexer;
	nn.children.push_back(knn);
	ptr = temp; }

	// match token PARSER
	if (!match(ptr, i_PARSER)) { goto no_match_0; } ptr++;

	// match parser
	{ node knn;
	if ((temp = s_parser(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_parser;
	nn.children.push_back(knn);
	ptr = temp; }

	// match token EOF
	if (!match(ptr, i_EOF)) { goto no_match_0; } ptr++;
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

int parser::s_lexer(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token
		{ node knn; if ((temp = s_token(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_token;
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

int parser::s_token(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match type
	{ node knn;
	if ((temp = s_type(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_type;
	nn.children.push_back(knn);
	ptr = temp; }

	// match token ID
	if (!match(ptr, i_ID)) { goto no_match_0; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }
	ptr++;

	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match lexer_group
		{ node knn; if ((temp = s_lexer_group(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_lexer_group;
		lnn.children.push_back(knn);
		lptr = temp; }

		ptr = lptr;
		nn = lnn;
	}

	// match token SEMICOLON
	if (!match(ptr, i_SEMICOLON)) { goto no_match_0; } ptr++;
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

int parser::s_lexer_group(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match token LP
	if (!match(ptr, i_LP)) { goto no_match_0; } ptr++;

	// match lexer_atom
	{ node knn;
	if ((temp = s_lexer_atom(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_lexer_atom;
	nn.children.push_back(knn);
	ptr = temp; }
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token SEPARATOR
		if (!match(lptr, i_SEPARATOR)) { break; }  lptr++;

		// match lexer_atom
		{ node knn; if ((temp = s_lexer_atom(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_lexer_atom;
		lnn.children.push_back(knn);
		lptr = temp; }

		ptr = lptr;
		nn = lnn;
	}

	// match token RP
	if (!match(ptr, i_RP)) { goto no_match_0; } ptr++;
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token MOD
		if (!match(lptr, i_MOD)) { break; }
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		 lptr++;
		ptr = lptr;
		nn = lnn;
		break;
	}
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

int parser::s_type(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match token TOKEN
	if (!match(ptr, i_TOKEN)) { goto no_match_0; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;
	*n = nn;
	return ptr;
	no_match_0:

	// ----------------------
	ptr = i;
	nn.clear();

	// match token SKIP
	if (!match(ptr, i_SKIP)) { goto no_match_1; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;
	*n = nn;
	return ptr;
	no_match_1:

	// ----------------------
	ptr = i;
	nn.clear();

	// match token PRIV
	if (!match(ptr, i_PRIV)) { goto no_match_2; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;
	*n = nn;
	return ptr;
	no_match_2:

	return -1;
}

int parser::s_lexer_atom(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token REV
		if (!match(lptr, i_REV)) { break; }
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		 lptr++;
		ptr = lptr;
		nn = lnn;
		break;
	}

	// match token ID
	if (!match(ptr, i_ID)) { goto no_match_0; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;
	*n = nn;
	return ptr;
	no_match_0:

	// ----------------------
	ptr = i;
	nn.clear();
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token REV
		if (!match(lptr, i_REV)) { break; }
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		 lptr++;
		ptr = lptr;
		nn = lnn;
		break;
	}

	// match token STRING
	if (!match(ptr, i_STRING)) { goto no_match_1; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;
	*n = nn;
	return ptr;
	no_match_1:

	// ----------------------
	ptr = i;
	nn.clear();
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token REV
		if (!match(lptr, i_REV)) { break; }
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		 lptr++;
		ptr = lptr;
		nn = lnn;
		break;
	}

	// match token LETTER
	if (!match(ptr, i_LETTER)) { goto no_match_2; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;

	// match token DOUBLEDOT
	if (!match(ptr, i_DOUBLEDOT)) { goto no_match_2; } ptr++;

	// match token LETTER
	if (!match(ptr, i_LETTER)) { goto no_match_2; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;
	*n = nn;
	return ptr;
	no_match_2:

	// ----------------------
	ptr = i;
	nn.clear();
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token REV
		if (!match(lptr, i_REV)) { break; }
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		 lptr++;
		ptr = lptr;
		nn = lnn;
		break;
	}

	// match token LETTER
	if (!match(ptr, i_LETTER)) { goto no_match_3; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }

	ptr++;
	*n = nn;
	return ptr;
	no_match_3:

	return -1;
}

int parser::s_parser(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match rule
		{ node knn; if ((temp = s_rule(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_rule;
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

int parser::s_rule(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match token RULE
	if (!match(ptr, i_RULE)) { goto no_match_0; } ptr++;

	// match token ID
	if (!match(ptr, i_ID)) { goto no_match_0; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }
	ptr++;

	// match alternative
	{ node knn;
	if ((temp = s_alternative(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_alternative;
	nn.children.push_back(knn);
	ptr = temp; }
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token SEPARATOR
		if (!match(lptr, i_SEPARATOR)) { break; }  lptr++;

		// match alternative
		{ node knn; if ((temp = s_alternative(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_alternative;
		lnn.children.push_back(knn);
		lptr = temp; }

		ptr = lptr;
		nn = lnn;
	}

	// match token SEMICOLON
	if (!match(ptr, i_SEMICOLON)) { goto no_match_0; } ptr++;
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

int parser::s_alternative(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match parser_group
		{ node knn; if ((temp = s_parser_group(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_parser_group;
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

int parser::s_parser_group(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();

	// match token LP
	if (!match(ptr, i_LP)) { goto no_match_0; } ptr++;

	// match parser_atom
	{ node knn;
	if ((temp = s_parser_atom(ptr, &knn)) <= ptr) { goto no_match_0; }
	knn.rule = r_parser_atom;
	nn.children.push_back(knn);
	ptr = temp; }
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match parser_atom
		{ node knn; if ((temp = s_parser_atom(lptr, &knn)) <= lptr) { break; }
		knn.rule = r_parser_atom;
		lnn.children.push_back(knn);
		lptr = temp; }

		ptr = lptr;
		nn = lnn;
	}

	// match token RP
	if (!match(ptr, i_RP)) { goto no_match_0; } ptr++;
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token MOD
		if (!match(lptr, i_MOD)) { break; }
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		 lptr++;
		ptr = lptr;
		nn = lnn;
		break;
	}
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

int parser::s_parser_atom(int i, node* n)
{
	register int ptr;
	node nn;
	int temp = -1;

	// ----------------------
	ptr = i;
	nn.clear();
	while (true)
	{
		register int lptr = ptr;
		node lnn = nn;

		// match token ASTMOD
		if (!match(lptr, i_ASTMOD)) { break; }
		{ node p(n_token, l.tokens[ptr]);
		lnn.children.push_back(p); }
		lptr++;
		ptr = lptr;
		nn = lnn;
		break;
	}

	// match token ID
	if (!match(ptr, i_ID)) { goto no_match_0; } 
	{ node p(n_token, l.tokens[ptr]);
	nn.children.push_back(p); }
	ptr++;
	*n = nn;
	return ptr;
	no_match_0:

	return -1;
}

}

