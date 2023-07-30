#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;
namespace yatta {

class exception
{
public:
	string error;
	int line;
	exception() : line(0) { }
	exception(string text, int ln) : error(text), line(ln) { }
};

enum token_types {
	ENDOFFILE,
	whitespace,
	number,
	add,
	mult,
	lp,
	rp
};

struct token
{
	token_types type;
	string type_name;
	string text;
	int line;
	bool skip;
};

struct a_digit {  static inline int match(string& str, int i); };
struct a_whitespace {  static inline int match(string& str, int i); };
struct a_number {  static inline int match(string& str, int i); };
struct a_add {  static inline int match(string& str, int i); };
struct a_mult {  static inline int match(string& str, int i); };
struct a_lp {  static inline int match(string& str, int i); };
struct a_rp {  static inline int match(string& str, int i); };

class lexer
{
public:
	vector<token> tokens;
	string filename;
	unsigned int lines;

	lexer() : lines (1), filename("line") { }
	void run(string data);
	string translate_tokens(token_types t);
};

class str
{
public:
	inline static int match(const string& what, const string& str, int i);
};

}

#endif

