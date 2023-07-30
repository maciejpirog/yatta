#ifndef LEXER_ATOMS_H
#define LEXER_ATOMS_H

#include <string>
#include <vector>

using namespace std;

enum group_type { g_obligatory, g_optional, g_repeat };
enum atom_type { a_character, a_string, a_token };

struct atom
{
	atom_type type;
	char character;
	bool reverse;
	string token_name;

	atom() : reverse(false), type(a_character) { }
};

struct group
{
	group_type type;
	vector<atom> atoms;

	group() : type(g_obligatory) { }
	group(char ch);
	group(string name);

	void add(char ch);
	void add(string name);
};

struct token_type
{
	string name;
	vector<group> groups;
	bool skip;
	bool priv;

	token_type() : skip(false), priv(false) { }
	token_type(string name, string text);
	token_type(string name, char ch);
	token_type(string name, char ch1, char ch2, bool rev = false);

	void add_group(group& g);
};

#endif


