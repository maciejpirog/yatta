#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "ast.h" 

using namespace std;
using namespace yatta;

int eval(node& n)
{
	int value = 0;
	if (n.rule == r_add_expr)
	{
		value = eval(n.children[0]);
		for (size_t i = 1; i < n.children.size(); i+=2)
		{
			if (n.children[i].t.text == "+") value += eval(n.children[i+1]);
			if (n.children[i].t.text == "-") value -= eval(n.children[i+1]);
		}
	}
	if (n.rule == r_mult_expr)
	{
		value = eval(n.children[0]);
		for (size_t i = 1; i < n.children.size(); i+=2)
		{
			if (n.children[i].t.text == "*") value *= eval(n.children[i+1]);
			if (n.children[i].t.text == "/") value /= eval(n.children[i+1]);
		}
	}
	if (n.type == n_token && n.t.type == number) return atoi(n.t.text.c_str());
	if (n.rule == r_base_expr)
	{
		value = eval(n.children[0]);
	}
	return value;
}

int main()
{
	cout << "\n  * welcome to the simple calculator!\n  * type an expression or q to quit.\n\n";
	for (;;)
	{
		string in;
		cout << ">";
		getline(cin, in);
		if (in == "quit" || in == "q") break;
		parser p;
		try {
			node n = p.run(in);
			cout << "result: " << eval(n.children[0]) << "\n";
		}
		catch (yatta::exception e)	{
			cout << e.error << "\n";
		}
	}
}
