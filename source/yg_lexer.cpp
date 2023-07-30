#include "yg_lexer.h"

namespace yatta {

inline int a_ALPHAL::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == 'a') { ptr++; }
	else if (k == 'b') { ptr++; }
	else if (k == 'c') { ptr++; }
	else if (k == 'd') { ptr++; }
	else if (k == 'e') { ptr++; }
	else if (k == 'f') { ptr++; }
	else if (k == 'g') { ptr++; }
	else if (k == 'h') { ptr++; }
	else if (k == 'i') { ptr++; }
	else if (k == 'j') { ptr++; }
	else if (k == 'k') { ptr++; }
	else if (k == 'l') { ptr++; }
	else if (k == 'm') { ptr++; }
	else if (k == 'n') { ptr++; }
	else if (k == 'o') { ptr++; }
	else if (k == 'p') { ptr++; }
	else if (k == 'q') { ptr++; }
	else if (k == 'r') { ptr++; }
	else if (k == 's') { ptr++; }
	else if (k == 't') { ptr++; }
	else if (k == 'u') { ptr++; }
	else if (k == 'v') { ptr++; }
	else if (k == 'w') { ptr++; }
	else if (k == 'x') { ptr++; }
	else if (k == 'y') { ptr++; }
	else if (k == 'z') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_ALPHAB::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == 'A') { ptr++; }
	else if (k == 'B') { ptr++; }
	else if (k == 'C') { ptr++; }
	else if (k == 'D') { ptr++; }
	else if (k == 'E') { ptr++; }
	else if (k == 'F') { ptr++; }
	else if (k == 'G') { ptr++; }
	else if (k == 'H') { ptr++; }
	else if (k == 'I') { ptr++; }
	else if (k == 'J') { ptr++; }
	else if (k == 'K') { ptr++; }
	else if (k == 'L') { ptr++; }
	else if (k == 'M') { ptr++; }
	else if (k == 'N') { ptr++; }
	else if (k == 'O') { ptr++; }
	else if (k == 'P') { ptr++; }
	else if (k == 'Q') { ptr++; }
	else if (k == 'R') { ptr++; }
	else if (k == 'S') { ptr++; }
	else if (k == 'T') { ptr++; }
	else if (k == 'U') { ptr++; }
	else if (k == 'V') { ptr++; }
	else if (k == 'W') { ptr++; }
	else if (k == 'X') { ptr++; }
	else if (k == 'Y') { ptr++; }
	else if (k == 'Z') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_NUM::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '0') { ptr++; }
	else if (k == '1') { ptr++; }
	else if (k == '2') { ptr++; }
	else if (k == '3') { ptr++; }
	else if (k == '4') { ptr++; }
	else if (k == '5') { ptr++; }
	else if (k == '6') { ptr++; }
	else if (k == '7') { ptr++; }
	else if (k == '8') { ptr++; }
	else if (k == '9') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_ID::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = a_ALPHAL::match(str, ptr)) >= ptr ) { ptr = temp; }
	else if ((temp = a_ALPHAB::match(str, ptr)) >= ptr ) { ptr = temp; }
	else if (k == '_') { ptr++; }
	else { return -1; }

	while (true)
	{
		if (ptr >= str.size())
			break;
		k = str.at(ptr);

		if ((temp = a_ALPHAL::match(str, ptr)) >= ptr ) { ptr = temp; }
		else if ((temp = a_ALPHAB::match(str, ptr)) >= ptr ) { ptr = temp; }
		else if (k == '_') { ptr++; }
		else if ((temp = a_NUM::match(str, ptr)) >= ptr ) { ptr = temp; }
		else { break; }
	}

	return ptr;
}

inline int a_LETTER::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '\'') { ptr++; }
	else { return -1; }

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = a_LETTERQUOTEQUOTE::match(str, ptr)) >= ptr ) { ptr = temp; }
	else if (k != '\'') { ptr++; }
	else { return -1; }

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '\'') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_STRING::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '"') { ptr++; }
	else { return -1; }

	while (true)
	{
		if (ptr >= str.size())
			break;
		k = str.at(ptr);

		if (k != '"') { ptr++; }
		else { break; }
	}

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '"') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_WHITESPACE::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '\n') { ptr++; }
	else if (k == '\r') { ptr++; }
	else if (k == '\t') { ptr++; }
	else if (k == ' ') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_C_END::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '\n') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_C_BODY::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	while (true)
	{
		if (ptr >= str.size())
			break;
		k = str.at(ptr);

		if (k != '\n') { ptr++; }
		else { break; }
	}

	return ptr;
}

inline int a_C_START::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("//", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_COMMENT::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = a_C_START::match(str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = a_C_BODY::match(str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = a_C_END::match(str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_LP::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '(') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_RP::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == ')') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_MOD::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '*') { ptr++; }
	else if (k == '?') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_LEXER::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("LEXER:", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_PARSER::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("PARSER:", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_TOKEN::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("token", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_PRIV::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("priv", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_SKIP::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("skip", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_RULE::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("rule", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_SEPARATOR::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '|') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_REV::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '~') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_SEMICOLON::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == ';') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_DOUBLEDOT::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("..", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	return ptr;
}

inline int a_ASTMOD::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '^') { ptr++; }
	else if (k == '!') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_LETTERQUOTEQUOTE::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = str::match("\\", str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '\'') { ptr++; }
	else if (k == 'n') { ptr++; }
	else if (k == 'r') { ptr++; }
	else if (k == 't') { ptr++; }
	else if (k == '\\') { ptr++; }
	else { return -1; }

	return ptr;
}


void lexer::run(string data)
{
	int ptr = 0;
	int ptr2 = 0;

	while (true)
	{
		ptr2 = ptr - 1;
		int temp = 0;
		token current;
		if ((temp = a_ID::match(data, ptr)) >= ptr2)
		{
			current.type = i_ID;
			current.type_name = "ID";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_LETTER::match(data, ptr)) >= ptr2)
		{
			current.type = i_LETTER;
			current.type_name = "LETTER";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_STRING::match(data, ptr)) >= ptr2)
		{
			current.type = i_STRING;
			current.type_name = "STRING";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_WHITESPACE::match(data, ptr)) >= ptr2)
		{
			current.type = i_WHITESPACE;
			current.type_name = "WHITESPACE";
			current.skip = true;
			ptr2 = temp;
		}
		if ((temp = a_COMMENT::match(data, ptr)) >= ptr2)
		{
			current.type = i_COMMENT;
			current.type_name = "COMMENT";
			current.skip = true;
			ptr2 = temp;
		}
		if ((temp = a_LP::match(data, ptr)) >= ptr2)
		{
			current.type = i_LP;
			current.type_name = "LP";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_RP::match(data, ptr)) >= ptr2)
		{
			current.type = i_RP;
			current.type_name = "RP";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_MOD::match(data, ptr)) >= ptr2)
		{
			current.type = i_MOD;
			current.type_name = "MOD";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_LEXER::match(data, ptr)) >= ptr2)
		{
			current.type = i_LEXER;
			current.type_name = "LEXER";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_PARSER::match(data, ptr)) >= ptr2)
		{
			current.type = i_PARSER;
			current.type_name = "PARSER";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_TOKEN::match(data, ptr)) >= ptr2)
		{
			current.type = i_TOKEN;
			current.type_name = "TOKEN";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_PRIV::match(data, ptr)) >= ptr2)
		{
			current.type = i_PRIV;
			current.type_name = "PRIV";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_SKIP::match(data, ptr)) >= ptr2)
		{
			current.type = i_SKIP;
			current.type_name = "SKIP";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_RULE::match(data, ptr)) >= ptr2)
		{
			current.type = i_RULE;
			current.type_name = "RULE";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_SEPARATOR::match(data, ptr)) >= ptr2)
		{
			current.type = i_SEPARATOR;
			current.type_name = "SEPARATOR";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_REV::match(data, ptr)) >= ptr2)
		{
			current.type = i_REV;
			current.type_name = "REV";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_SEMICOLON::match(data, ptr)) >= ptr2)
		{
			current.type = i_SEMICOLON;
			current.type_name = "SEMICOLON";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_DOUBLEDOT::match(data, ptr)) >= ptr2)
		{
			current.type = i_DOUBLEDOT;
			current.type_name = "DOUBLEDOT";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_ASTMOD::match(data, ptr)) >= ptr2)
		{
			current.type = i_ASTMOD;
			current.type_name = "ASTMOD";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_LETTERQUOTEQUOTE::match(data, ptr)) >= ptr2)
		{
			current.type = i_LETTERQUOTEQUOTE;
			current.type_name = "LETTERQUOTEQUOTE";
			current.skip = false;
			ptr2 = temp;
		}

		if (ptr2 > ptr)
		{
			current.text = data.substr(ptr, ptr2-ptr);
			for (int iter = 0; iter < current.text.size(); iter++)
				if (current.text.at(iter) == '\n') lines++;
			current.line = lines;
			if (!current.skip)
				tokens.push_back(current);
		}

		if ((ptr2>0 && ptr2 >= data.size()) || ptr >= data.size() - 1)
		{
			token tok_eof;
			tok_eof.type = i_EOF;
			tok_eof.type_name = "EOF";
			tok_eof.line = lines;
			tokens.push_back(tok_eof);
			return;
		}
		if (ptr2 <= ptr && ptr <= data.size() - 1)
		{
			stringstream strerr;
			strerr << filename << ":" << lines << ": unknown token '" << data.at(ptr) << "'";
			throw exception(strerr.str(), lines);
		}
		ptr = ptr2;
	}
}

inline int str::match(const string& what, const string& str, int i)
{
	for (size_t j = 0; j < what.size(); j++)
		if (what.at(j) != str.at(i+j)) return -1;
	return i + what.size();
}

string lexer::translate_tokens(token_types t)
{
	if (t == i_ID) return "ID";
	if (t == i_LETTER) return "LETTER";
	if (t == i_STRING) return "STRING";
	if (t == i_WHITESPACE) return "WHITESPACE";
	if (t == i_COMMENT) return "COMMENT";
	if (t == i_LP) return "LP";
	if (t == i_RP) return "RP";
	if (t == i_MOD) return "MOD";
	if (t == i_LEXER) return "LEXER";
	if (t == i_PARSER) return "PARSER";
	if (t == i_TOKEN) return "TOKEN";
	if (t == i_PRIV) return "PRIV";
	if (t == i_SKIP) return "SKIP";
	if (t == i_RULE) return "RULE";
	if (t == i_SEPARATOR) return "SEPARATOR";
	if (t == i_REV) return "REV";
	if (t == i_SEMICOLON) return "SEMICOLON";
	if (t == i_DOUBLEDOT) return "DOUBLEDOT";
	if (t == i_ASTMOD) return "ASTMOD";
	if (t == i_LETTERQUOTEQUOTE) return "LETTERQUOTEQUOTE";
	if (t == i_EOF) return "END OF FILE";
	return "";
}

}

