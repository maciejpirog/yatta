#include <iostream>
#include <string>
#include "lexgen.h"
#include "parsegen.h"
#include "yg_lexer.h"
#include "yg_parser.h"
#include "yg_ast.h"
#include "walkers.h"
#include "exceptions.h"

using namespace std;
using namespace yatta;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "usage: yatta filename\n";
		return -1;
	}
	
	fstream input(argv[1]);
	if (!input)
	{
		cerr << "error: cannot open file " << argv[1] << "\n";
		return -1;
	}
	
	string in;
	while (!input.eof())
		in += input.get();
	input.close();

	parser p;
	p.filename = argv[1];

	try
	{
		node n = p.run(in);
		lexgen_cpp lg;
		parsegen_cpp pg;
		lexer_input::lex(n.children[0], &lg);
		parser_input::parse(n.children[1], &pg);
		lg.gen();
		pg.gen();
	}
	catch (yatta::exception e)
	{
		cerr << e.error << "\n";
		return -1;
	}
	catch (ex e)
	{
		cerr << e.error << "\n";
		return -1;
	}
}

