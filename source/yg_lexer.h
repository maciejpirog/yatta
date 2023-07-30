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
	i_EOF,
	i_ID,
	i_LETTER,
	i_STRING,
	i_WHITESPACE,
	i_COMMENT,
	i_LP,
	i_RP,
	i_MOD,
	i_LEXER,
	i_PARSER,
	i_TOKEN,
	i_PRIV,
	i_SKIP,
	i_RULE,
	i_SEPARATOR,
	i_REV,
	i_SEMICOLON,
	i_DOUBLEDOT,
	i_ASTMOD,
	i_LETTERQUOTEQUOTE
};

struct token
{
	token_types type;
	string type_name;
	string text;
	int line;
	bool skip;
};

struct a_ALPHAL {  static inline int match(string& str, int i); };
struct a_ALPHAB {  static inline int match(string& str, int i); };
struct a_NUM {  static inline int match(string& str, int i); };
struct a_ID {  static inline int match(string& str, int i); };
struct a_LETTER {  static inline int match(string& str, int i); };
struct a_STRING {  static inline int match(string& str, int i); };
struct a_WHITESPACE {  static inline int match(string& str, int i); };
struct a_C_END {  static inline int match(string& str, int i); };
struct a_C_BODY {  static inline int match(string& str, int i); };
struct a_C_START {  static inline int match(string& str, int i); };
struct a_COMMENT {  static inline int match(string& str, int i); };
struct a_LP {  static inline int match(string& str, int i); };
struct a_RP {  static inline int match(string& str, int i); };
struct a_MOD {  static inline int match(string& str, int i); };
struct a_LEXER {  static inline int match(string& str, int i); };
struct a_PARSER {  static inline int match(string& str, int i); };
struct a_TOKEN {  static inline int match(string& str, int i); };
struct a_PRIV {  static inline int match(string& str, int i); };
struct a_SKIP {  static inline int match(string& str, int i); };
struct a_RULE {  static inline int match(string& str, int i); };
struct a_SEPARATOR {  static inline int match(string& str, int i); };
struct a_REV {  static inline int match(string& str, int i); };
struct a_SEMICOLON {  static inline int match(string& str, int i); };
struct a_DOUBLEDOT {  static inline int match(string& str, int i); };
struct a_ASTMOD {  static inline int match(string& str, int i); };
struct a_LETTERQUOTEQUOTE {  static inline int match(string& str, int i); };

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

