#ifndef LEXGEN_H
#define LEXGEN_H

#include <fstream>
#include <vector>
#include <iostream>
#include <string>

#include "lexer_atoms.h"

using namespace std;

class lexgen
{
protected:
	ofstream header;
	ofstream source;	
public:
	vector<token_type> token_types;

	virtual void gen() = 0;
}; 

class lexgen_cpp : public lexgen
{
public:
	virtual void gen();

protected:
	void gen_headers();
	void gen_lexer();
	void gen_token_types_classes();
	void gen_atom(atom a, const size_t w, const string prefix);
	void gen_exception();
};

#endif


