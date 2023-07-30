#include "lexer.h"

namespace yatta {

inline int a_digit::match(string& str, int i)
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

inline int a_whitespace::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == 'q') { ptr++; }
	else if (k == ' ') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_number::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if ((temp = a_digit::match(str, ptr)) >= ptr ) { ptr = temp; }
	else { return -1; }

	while (true)
	{
		if (ptr >= str.size())
			break;
		k = str.at(ptr);

		if ((temp = a_digit::match(str, ptr)) >= ptr ) { ptr = temp; }
		else { break; }
	}

	return ptr;
}

inline int a_add::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '+') { ptr++; }
	else if (k == '-') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_mult::match(string& str, int i)
{
	int ptr = i;
	int temp = -1;
	register char k;

	if (ptr >= str.size())
		return -1;

	k = str.at(ptr);
	if (k == '*') { ptr++; }
	else if (k == '/') { ptr++; }
	else { return -1; }

	return ptr;
}

inline int a_lp::match(string& str, int i)
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

inline int a_rp::match(string& str, int i)
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


void lexer::run(string data)
{
	int ptr = 0;
	int ptr2 = 0;

	while (true)
	{
		ptr2 = ptr - 1;
		int temp = 0;
		token current;
		if ((temp = a_whitespace::match(data, ptr)) >= ptr2)
		{
			current.type = whitespace;
			current.type_name = "whitespace";
			current.skip = true;
			ptr2 = temp;
		}
		if ((temp = a_number::match(data, ptr)) >= ptr2)
		{
			current.type = number;
			current.type_name = "number";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_add::match(data, ptr)) >= ptr2)
		{
			current.type = add;
			current.type_name = "add";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_mult::match(data, ptr)) >= ptr2)
		{
			current.type = mult;
			current.type_name = "mult";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_lp::match(data, ptr)) >= ptr2)
		{
			current.type = lp;
			current.type_name = "lp";
			current.skip = false;
			ptr2 = temp;
		}
		if ((temp = a_rp::match(data, ptr)) >= ptr2)
		{
			current.type = rp;
			current.type_name = "rp";
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
			tok_eof.type = ENDOFFILE;
			tok_eof.type_name = "ENDOFFILE";
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
	if (t == whitespace) return "whitespace";
	if (t == number) return "number";
	if (t == add) return "add";
	if (t == mult) return "mult";
	if (t == lp) return "lp";
	if (t == rp) return "rp";
	if (t == ENDOFFILE) return "END OF FILE";
	return "";
}

}

