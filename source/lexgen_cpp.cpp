#include "lexgen.h"
#include "parsegen.h"
#include "exceptions.h"

using namespace std;

void lexgen_cpp::gen()
{
	header.open("lexer.h");
	if(!header) throw ex("error: cannot create file lexer.h");
	source.open("lexer.cpp");
	if(!source) throw ex("error: cannot create file lexer.cpp");
	
	header << "#ifndef LEXER_H\n";
	header << "#define LEXER_H\n\n";
	
	gen_headers();
	
	source << "namespace yatta {\n\n";
	header << "namespace yatta {\n\n";
	
	gen_exception();
	
	gen_token_types_classes();
	gen_lexer();
	
	source << "\n}\n\n";
	header << "\n}\n\n";
	
	header <<"#endif\n\n";
	
	header.close();
	source.close();
}

void lexgen_cpp::gen_exception()
{
	header << "class exception\n";
	header << "{\n";
	header << "public:\n";
	header << "\tstring error;\n";
	header << "\tint line;\n";
	header << "\texception() : line(0) { }\n";
	header << "\texception(string text, int ln) : error(text), line(ln) { }\n";
	header << "};\n\n";
}

void lexgen_cpp::gen_headers()
{
	header << "#include <string>\n#include <vector>\n";
	header << "#include <sstream>";
	header << "\n\nusing namespace std;\n";
	
	source << "#include \"lexer.h\"\n\n";
}

void lexgen_cpp::gen_lexer()
{
	header << "\nclass lexer\n{\n";
	header << "public:\n";
	header << "\tvector<token> tokens;\n";
	header << "\tstring filename;\n";
	header << "\tunsigned int lines;\n\n";
	header << "\tlexer() : lines (1), filename(\"line\") { }\n";
	header << "\tvoid run(string data);\n";
	header << "\tstring translate_tokens(token_types t);\n";
	header << "};\n\n";
	
	header << "class str\n{\npublic:\n";
	header << "\tinline static int match(const string& what, const string& str, int i);\n};\n";
	
	source << "\nvoid lexer::run(string data)\n{\n";
	source << "\tint ptr = 0;\n";
	source << "\tint ptr2 = 0;\n";
	source << "\n\twhile (true)\n\t{\n";
	
	source << "\t\tptr2 = ptr - 1;\n";
	source << "\t\tint temp = 0;\n";
	source << "\t\ttoken current;\n";
	
	for (size_t t = 0; t < token_types.size(); t++)
	{
		if (token_types[t].priv)
			continue;
		source << "\t\tif ((temp = a_" << token_types[t].name << "::match(data, ptr)) >= ptr2)\n";
		source << "\t\t{\n";
		source << "\t\t\tcurrent.type = " << token_types[t].name << ";\n";
		source << "\t\t\tcurrent.type_name = \"" << token_types[t].name << "\";\n";
		if (token_types[t].skip)
			source << "\t\t\tcurrent.skip = true;\n";
		else
			source << "\t\t\tcurrent.skip = false;\n";
		source << "\t\t\tptr2 = temp;\n";
		source << "\t\t}\n";
	}
	
	source << "\n\t\tif (ptr2 > ptr)\n\t\t{\n";
	source << "\t\t\tcurrent.text = data.substr(ptr, ptr2-ptr);\n";
	source << "\t\t\tfor (int iter = 0; iter < current.text.size(); iter++)\n\t\t\t\tif (current.text.at(iter) == \'\\n\') lines++;\n";
	source << "\t\t\tcurrent.line = lines;\n";
	source << "\t\t\tif (!current.skip)\n";
	source << "\t\t\t\ttokens.push_back(current);\n";
	source << "\t\t}\n\n";
	
	source << "\t\tif ((ptr2>0 && ptr2 >= data.size()) || ptr >= data.size() - 1)\n";
	source << "\t\t{\n";
	source << "\t\t\ttoken tok_eof;\n";
	source << "\t\t\ttok_eof.type = ENDOFFILE;\n";
	source << "\t\t\ttok_eof.type_name = \"ENDOFFILE\";\n";
	source << "\t\t\ttok_eof.line = lines;\n";
	source << "\t\t\ttokens.push_back(tok_eof);\n";
	source << "\t\t\treturn;\n\t\t}\n";
	source << "\t\tif (ptr2 <= ptr && ptr <= data.size() - 1)\n";
	source << "\t\t{\n";
	source << "\t\t\tstringstream strerr;\n";
	source << "\t\t\tstrerr << filename << \":\" << lines << \": unknown token \'\" << data.at(ptr) << \"\'\";\n";
	source << "\t\t\tthrow exception(strerr.str(), lines);\n";
	source << "\t\t}\n";
	
	source << "\t\tptr = ptr2;\n";
	source << "\t}\n";
	source << "}\n";
	
	source << "\ninline int str::match(const string& what, const string& str, int i)\n";
	source << "{\n";
	source << "\tfor (size_t j = 0; j < what.size(); j++)\n";
	source << "\t\tif (what.at(j) != str.at(i+j)) return -1;\n";
	source << "\treturn i + what.size();\n";
	source << "}\n";
	
	source << "\nstring lexer::translate_tokens(token_types t)\n";
	source << "{\n";
	for (size_t t = 0; t < token_types.size(); t++)
	{
		if (!token_types[t].priv)
			source << "\tif (t == " << token_types[t].name << ") return \"" << token_types[t].name << "\";\n";
	}
	source << "\tif (t == ENDOFFILE) return \"END OF FILE\";\n";
	source << "\treturn \"\";\n";
	source << "}\n";
	
}

void lexgen_cpp::gen_token_types_classes()
{
	header << "enum token_types {\n";
	header << "\tENDOFFILE,\n";
	for (size_t ty = 0; ty < token_types.size(); ty++)
	{
		if (token_types[ty].priv)
			continue;
		header << "\t" << token_types[ty].name;
		if (ty < token_types.size() - 1)
			header << ',';
		header << "\n";
	}
	header << "};\n\n";
	
	header << "struct token\n{\n";
	header << "\ttoken_types type;\n";
	header << "\tstring type_name;\n";
	header << "\tstring text;\n";
	header << "\tint line;\n";
	header << "\tbool skip;\n";
	header << "};\n\n";
	
	for (size_t t = 0; t < token_types.size(); t++)
	{	
		header << "struct a_";
		header << token_types[t].name;
		header << " { ";
		header << " static inline int match(string& str, int i); ";
		header << "};\n";

		source << "inline int a_";
		source << token_types[t].name;
		source << "::match(string& str, int i)\n{\n";
		source << "\tint ptr = i;\n\tint temp = -1;\n";
		source << "\tregister char k;\n\n";
		
		for (size_t g = 0; g < token_types[t].groups.size(); g++)
		{
			if (token_types[t].groups[g].type == g_obligatory
				|| token_types[t].groups[g].type == g_optional)
			{
				source << "\tif (ptr >= str.size())\n\t\t";
				source << "return -1;\n";
				source << "\n\tk = str.at(ptr);\n";
				for (size_t a = 0; a < token_types[t].groups[g].atoms.size(); a++ )
					gen_atom(token_types[t].groups[g].atoms[a], a, "");
				
				if (token_types[t].groups[g].type == g_obligatory)
					source << "\telse { return -1; }\n";
				source << "\n";
			}
			
			if (token_types[t].groups[g].type == g_repeat)
			{
				source << "\twhile (true)\n\t{\n";
				source << "\t\tif (ptr >= str.size())\n\t\t\t";
				source << "break;\n";
				source << "\t\tk = str.at(ptr);\n\n";
				for (size_t a = 0; a < token_types[t].groups[g].atoms.size(); a++ )
					gen_atom(token_types[t].groups[g].atoms[a], a, "\t");
				
				source << "\t\telse { break; }\n\t}\n";
				source << "\n";
			}
		}
		
		source << "\treturn ptr;\n";
		source << "}\n\n";
	}
}

void lexgen_cpp::gen_atom(atom a, const size_t w, const string prefix)
{
	if (a.type == a_character)
	{
		source << prefix << "\t";
		if (w > 0) source << "else ";
		if (!a.reverse)
			source << "if (k == \'";
		else
			source << "if (k != \'";
		
		if (a.character == '\\')
			source << "\\\\";
		else if (a.character == '\"')
			source << '"';
		else if (a.character == '\'')
			source << "\\\'";
		else if (a.character == '\n')
			source << "\\n";
		else if (a.character == '\t')
			source << "\\t";
		else if (a.character == '\r')
			source << "\\r";
		else
			source << a.character;
		source << "\') { ptr++; }\n";
	}
	else if (a.type == a_string)
	{
		source << prefix << "\t";
		if(w > 0) source << "else ";
		source << "if ((temp = ";
		if (!a.reverse)
			source << "str::match(\"";
		else
			source << "str::match(\"";
		
		for (size_t i = 0; i < a.token_name.size(); i++)
		{
			if (a.token_name.at(i) == '\\')
				source << "\\\\";
			else if (a.token_name.at(i) == '\"')
				source << '"';
			else if (a.token_name.at(i) == '\'')
				source << "\\\'";
			else if (a.token_name.at(i) == '\n')
				source << "\\n";
			else if (a.token_name.at(i) == '\t')
				source << "\\t";
			else if (a.token_name.at(i) == '\r')
				source << "\\r";
			else
				source << a.token_name.at(i);
		}
		
		if (!a.reverse)
			source << "\", str, ptr)) >= ptr ) { ptr = temp; }\n";
		else
			source << "\", str, ptr)) < ptr ) { ptr++; }\n";
	}
	else // a_token
	{
		source << prefix << "\t";
		if(w > 0) source << "else ";
		source << "if ((temp = a_"<< a.token_name;
		if (!a.reverse)
			source << "::match(str, ptr)) >= ptr ) { ptr = temp; }\n";
		else
			source << "::match(str, ptr)) < ptr ) { ptr = temp; }\n";
	}
}
