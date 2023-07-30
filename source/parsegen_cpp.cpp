#include "parsegen.h"
#include "exceptions.h"

void parsegen_cpp::gen()
{
	header.open("parser.h");
	if (!header) throw ex("error: cannot create file parser.h");
	source.open("parser.cpp");
	if (!source) throw ex("error: cannot create file parser.cpp");
	
	header << "#ifndef PARSER_H\n";
	header << "#define PARSER_H\n\n";
	
	gen_headers();
	
	source << "namespace yatta {\n\n";
	header << "namespace yatta {\n\n";
	
	gen_parser();
	gen_prods();
	
	source << "}\n\n";
	header << "}\n\n";
	
	header <<"#endif\n\n";
	
	header.close();
	source.close();
	
	// generates ast.h
	gen_ast();
}

void parsegen_cpp::gen_headers()
{
	header << "#include \"lexer.h\"\n";
	header << "#include \"ast.h\"\n\n";
	source << "#include <sstream>\n";
	source << "#include \"parser.h\"\n\n";
}

void parsegen_cpp::gen_parser()
{	
	header << "class parser\n";
	header << "{\nprotected:\n";
	header << "\tlexer l;\n";
	header << "\npublic:\n";
	header << "\tstring filename;\n";
	header << "\tparser() : filename(\"line\") { }\n";
	header << "\tnode run(string data);\n";
	header << "\tinline bool match(int ptr, token_types type);\n";
	header << "\tstatic string translate_rules(rule_types r);\n";
	
	for (size_t i=0; i < rules.size(); i++)
	{
		header << "\tint s_"<< rules[i].name << "(int i, node* n);\n";
	}
	
	header << "\n\t// for error handling:\n";
	header << "\ttoken_types e_expected;\n";
	header << "\ttoken_types e_found;\n";
	header << "\tint e_line;\n";
	header << "\tstring get_error();\n";
	
	header << "};\n\n";
	
	source << "inline bool parser::match(int ptr, token_types type)\n";
	source << "{\n";
	source << "\tif (l.tokens[ptr].type == type)\n";
	source << "\t{\n";
	source << "\t\treturn true;\n";
	source << "\t}\n";
	source << "\telse\n";
	source << "\t{\n";
	source << "\t\te_expected = type;\n";
	source << "\t\te_found = l.tokens[ptr].type;\n";
	source << "\t\te_line = l.tokens[ptr].line;\n";
	source << "\t\treturn false;\n";
	source << "\t}\n";
	source << "}\n\n";
	
	source << "node parser::run(string data)\n";
	source << "{\n";
	source << "\tl.filename = filename;\n";
	source << "\tl.run(data);\n";
	source << "\tnode t;\n";
	source << "\tif(s_" << rules[0].name << "(0, &t) < 0)\n";
	source << "\t\tthrow exception(get_error(), e_line);\n";
	source << "\telse\n";
	source << "\t{\n";
	source << "\t\tt.rule = r_" << rules[0].name << ";\n";
	source << "\t\treturn t;\n";
	source << "\t}\n";
	source << "}\n\n";
	
	source << "string parser::get_error()\n";
	source << "{\n";
	source << "\tstd::stringstream str;\n";
	source << "\tstr << filename << \":\";\n";
	source << "\tstr << e_line;\n";
	source << "\tstr << \": expected token: \";\n";
	source << "\tstr << l.translate_tokens(e_expected);\n";
	source << "\tstr << \" found: \";\n";
	source << "\tstr << l.translate_tokens(e_found);\n";
	source << "\tstr << \"\";\n";
	source << "\treturn str.str();\n";
	source << "}\n\n";
	
	source << "string parser::translate_rules(rule_types r)\n";
	source << "{\n";
	for (size_t t = 0; t < rules.size(); t++)
	{
		source << "\tif (r == r_" << rules[t].name << ") return \"" << rules[t].name << "\";\n";
	}
	source << "\treturn \"\";\n";
	source << "}\n\n";
}

void parsegen_cpp::gen_prods()
{
	for (size_t i = 0; i < rules.size(); i++)
	{
		source << "int parser::s_"<< rules[i].name << "(int i, node* n)\n";
		source << "{\n";
		source << "\tregister int ptr;\n";
		source << "\tnode nn;\n";
		source << "\tint temp = -1;\n\n";
		
		for (size_t j = 0; j < rules[i].alternatives.size(); j++)
		{
			gen_alts(rules[i].alternatives[j], j);
		}
		
		source << "\treturn -1;\n";
		source << "}\n\n";
	}
}

void parsegen_cpp::gen_alts(alternative a, int k)
{
	source << "\t// ----------------------\n";
	source << "\tptr = i;\n";
	source << "\tnn.clear();\n";
	
	for (size_t i = 0; i < a.groups.size(); i++)
	{
		gen_groups(a.groups[i], k);
	}
	
	source << "\t*n = nn;\n";
	source << "\treturn ptr;\n";
	source << "\tno_match_" << k << ":\n\n";
}

void parsegen_cpp::gen_groups(parse_group g, int k)
{
	if (g.type == g_obligatory)
	{
		for (size_t i = 0; i < g.atoms.size(); i++)
		{
			if (g.atoms[i].type == p_token)
			{
				source << "\n\t// match token " << g.atoms[i].name;
				source << "\n\tif (!match(ptr, " << g.atoms[i].name << ")) { goto no_match_" << k << "; } ";
				if (!g.atoms[i].ignore)
					if (!g.atoms[i].root)
						source << "\n\t{ node p(n_token, l.tokens[ptr]);\n\tnn.children.push_back(p); }\n\n\t";
					else
						source << "\n\t{ nn.t = l.tokens[ptr];\n\tnn.type = n_token; }\n\t";
				source << "ptr++;\n";
			}
			else // p_rule
			{
				source << "\n\t// match " << g.atoms[i].name;
				source << "\n\t{ node knn;\n\tif ((temp = s_" << g.atoms[i].name << "(ptr, &knn)) <= ptr) { goto no_match_" << k << "; }";
				source << "\n\tknn.rule = r_" << g.atoms[i].name << ";";
				source << "\n\tnn.children.push_back(knn);\n\tptr = temp; }\n";
			}
		}
	}
	else if (g.type == g_optional)
	{
		source << "\twhile (true)\n";
		source << "\t{\n";
		source << "\t\tregister int lptr = ptr;\n";
		source << "\t\tnode lnn = nn;\n";
		for (size_t i = 0; i < g.atoms.size(); i++)
		{
			if (g.atoms[i].type == p_token)
			{
				source << "\n\t\t// match token " << g.atoms[i].name;
				source << "\n\t\tif (!match(lptr, " << g.atoms[i].name << ")) { break; }";
				if (!g.atoms[i].ignore)
					if (!g.atoms[i].root)
						source << "\n\t\t{ node p(n_token, l.tokens[ptr]);\n\t\tlnn.children.push_back(p); }\n\t\t";
					else
						source << "\n\t\t{ lnn.t = l.tokens[ptr];\n\t\tlnn.type = n_token; }\n\t\t";
				source << " lptr++;\n";
			}
			else // p_rule
			{
				source << "\n\t\t// match " << g.atoms[i].name;
				source << "\n\t\tif ((temp = s_" << g.atoms[i].name << "(lptr)) <= lptr) { break; } lptr = temp;\n";
			}
		}
		source << "\t\tptr = lptr;\n";
		source << "\t\tnn = lnn;\n";
		source << "\t\tbreak;\n";
		source << "\t}\n";
	}
	else if (g.type == g_repeat)
	{
		source << "\twhile (true)\n";
		source << "\t{\n";
		source << "\t\tregister int lptr = ptr;\n";
		source << "\t\tnode lnn = nn;\n";
		for (size_t i = 0; i < g.atoms.size(); i++)
		{
			if (g.atoms[i].type == p_token)
			{
				source << "\n\t\t// match token " << g.atoms[i].name;
				source << "\n\t\tif (!match(lptr, " << g.atoms[i].name << ")) { break; } ";
				if(!g.atoms[i].ignore)
					source << "\n\t\t{ node p(n_token, l.tokens[ptr]);\n\t\tlnn.children.push_back(p); }\n\t\t";
				source << " lptr++;\n";
			}
			else // p_rule
			{
				source << "\n\t\t// match " << g.atoms[i].name;
				source << "\n\t\t{ node knn; if ((temp = s_" << g.atoms[i].name << "(lptr, &knn)) <= lptr) { break; }";
				source << "\n\t\tknn.rule = r_" << g.atoms[i].name << ";";
				source << "\n\t\tlnn.children.push_back(knn);\n\t\tlptr = temp; }\n\n";
			}
		}
		source << "\t\tptr = lptr;\n";
		source << "\t\tnn = lnn;\n";
		source << "\t}\n";
	}
}

void parsegen_cpp::gen_ast()
{
	ofstream ast_header;
	ast_header.open("ast.h");
	if(!ast_header) throw ex("error: cannot create file ast.h");
	
	ast_header << "#ifndef AST_H\n";
	ast_header << "#define AST_H\n\n";
	
	ast_header << "#include <vector>\n";
	ast_header << "#include \"parser.h\"\n\n";
	
	ast_header << "using namespace std;\n\n";
	
	ast_header << "namespace yatta {\n\n";

	ast_header << "enum rule_types {\n";
	for (size_t ty = 0; ty < rules.size(); ty++)
	{
		ast_header << "\tr_" << rules[ty].name;
		if (ty < rules.size() - 1)
			ast_header << ',';
		ast_header << "\n";
	}
	ast_header << "};\n\n";
	
	ast_header << "enum node_type { n_token, n_rule };\n\n";

	ast_header << "struct node\n";
	ast_header << "{\n";
	ast_header << "\tnode_type type;\n";
	ast_header << "\ttoken t;\n";
	ast_header << "\trule_types rule;\n";
	ast_header << "\tvector<node> children;\n\n";
		
	ast_header << "\tnode(node_type tp, token tt) : type(tp), t(tt) { }\n";
	ast_header << "\tnode() : type(n_token) { }\n";
	ast_header << "\tvoid clear() { children.clear(); type = n_rule; }\n";
	ast_header << "};\n\n";


	ast_header << "}\n\n";

	ast_header << "#endif\n\n";

	ast_header.close();
}


