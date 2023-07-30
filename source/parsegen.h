#ifndef PARSEGEN_H
#define PARSEGEN_H

#include <fstream>
#include <vector>
#include <iostream>
#include <string>

#include "lexgen.h"
#include "parser_atoms.h"

using namespace std;

class parsegen
{
protected:
	ofstream header;
	ofstream source;
public:
	vector<rule> rules;

	virtual void gen() = 0;
};

class parsegen_cpp : public parsegen
{
public:
	virtual void gen();

protected:
	void gen_headers();
	void gen_parser();
	void gen_prods();
	void gen_alts(alternative a, int k);
	void gen_groups(parse_group g, int k);
	void gen_ast();
};

#endif

