/*
The makeparser function is responsible for creating
the yatta grammar files (.yg) lexer and parser.
*/

#include "lexgen.h"
#include "parsegen.h"
#include "parser_atoms.h"
#include "lexer_atoms.h"

void makeparser();

int main()
{
	makeparser();
}

void makeparser()
{
	// LEXER:
	lexgen_cpp lg;
	
	{
		// literals
		group sq('\"');
		atom sb; sb.type = a_character; sb.character = '\"'; sb.reverse = true;
		group sbg; sbg.type = g_repeat; sbg.atoms.push_back(sb);
		token_type string;
		string.add_group(sq);
		string.add_group(sbg);
		string.add_group(sq);
		string.name = "STRING";
		
		group lq('\'');
		atom lb; lb.type = a_character; lb.character = '\''; lb.reverse = true;
		group lbg;  lbg.add("LETTERQUOTEQUOTE"); lbg.atoms.push_back(lb);
		token_type letter;
		letter.add_group(lq);
		letter.add_group(lbg);
		letter.add_group(lq);
		letter.name = "LETTER";
		
		// identifier
		token_type alphal("ALPHAL", 'a', 'z'); alphal.priv = true;
		token_type alphab("ALPHAB", 'A', 'Z'); alphab.priv = true;
		token_type num("NUM", '0', '9'); num.priv = true;
		
		group idbg; idbg.add("ALPHAL");  idbg.add("ALPHAB");  idbg.add('_');
		group ideg; ideg.add("ALPHAL");  ideg.add("ALPHAB");  ideg.add('_'); ideg.add("NUM"); ideg.type = g_repeat;
		
		token_type id;
		id.name = "ID";
		id.add_group(idbg); id.add_group(ideg);
	
		lg.token_types.push_back(alphal);
		lg.token_types.push_back(alphab);
		lg.token_types.push_back(num);
		lg.token_types.push_back(id);
		lg.token_types.push_back(letter);
		lg.token_types.push_back(string);
		
		// whitespce
		group ws;
		ws.add('\n');
		ws.add('\r');
		ws.add('\t');
		ws.add(' ');
		token_type wst;
		wst.skip = true;
		wst.add_group(ws);
		wst.name = "WHITESPACE";
		lg.token_types.push_back(wst);
		
		// comment
		token_type c_start("C_START", "//"); c_start.priv = true;
		atom cb; cb.type = a_character; cb.character = '\n'; cb.reverse = true;
		group cbg; cbg.type = g_repeat; cbg.atoms.push_back(cb);
		token_type c_body; c_body.add_group(cbg); c_body.priv = true;
		c_body.name = "C_BODY";
		token_type c_end("C_END", '\n'); c_end.priv = true;
		token_type comment;
		comment.name = "COMMENT";
		group c1("C_START");
		group c2("C_BODY");
		group c3("C_END");
		comment.add_group(c1);
		comment.add_group(c2);
		comment.add_group(c3);
		comment.skip = true;
		lg.token_types.push_back(c_end);
		lg.token_types.push_back(c_body);
		lg.token_types.push_back(c_start);
		lg.token_types.push_back(comment);
		
		// parens
		token_type oblp("LP", '('); lg.token_types.push_back(oblp);
		token_type obrp("RP", ')'); lg.token_types.push_back(obrp);
		group modg; modg.add('*'); modg.add('?');  
		token_type mod; mod.add_group(modg);
		mod.name = "MOD";
		lg.token_types.push_back(mod);
		
		// headers
		token_type lexer("LEXER", "LEXER:"); lg.token_types.push_back(lexer);
		token_type parser("PARSER", "PARSER:"); lg.token_types.push_back(parser);
		token_type token("TOKEN", "token"); lg.token_types.push_back(token);
		token_type priv("PRIV", "priv"); lg.token_types.push_back(priv);
		token_type skip("SKIP", "skip"); lg.token_types.push_back(skip);
		token_type rule("RULE", "rule"); lg.token_types.push_back(rule);
		
		// single symbols
		token_type separator("SEPARATOR", '|'); lg.token_types.push_back(separator);
		token_type rev("REV", '~'); lg.token_types.push_back(rev);
		token_type semicolon("SEMICOLON", ';'); lg.token_types.push_back(semicolon);
		token_type doubledot("DOUBLEDOT", ".."); lg.token_types.push_back(doubledot);
		token_type astmod;
		group astmodg; astmodg.add('^'); astmodg.add('!');
		astmod.add_group(astmodg);
		astmod.name = "ASTMOD";
		lg.token_types.push_back(astmod);
		token_type letterquotequote("LETTERQUOTEQUOTE", "\\");
		group lqq; lqq.add('\''); lqq.add('n'); lqq.add('r'); lqq.add('t'); lqq.add('\\');
		letterquotequote.add_group(lqq);
		lg.token_types.push_back(letterquotequote);
	}
	
	lg.gen();
	
	// PARSER:
	parsegen_cpp pg;
	
	{
		// main
		parse_group LG;
		LG.atoms.push_back(parse_atom(p_token, "LEXER", true));
		LG.atoms.push_back(parse_atom(p_rule, "lexer"));
		LG.atoms.push_back(parse_atom(p_token, "PARSER", true));
		LG.atoms.push_back(parse_atom(p_rule, "parser"));
		LG.atoms.push_back(parse_atom(p_token, "EOF", true));
		alternative maina(LG);
		rule mainr("main");
		mainr.alternatives.push_back(maina);
		pg.rules.push_back(mainr);
		
		// lexer
		parse_group LEX;
		LEX.type = g_repeat;
		LEX.atoms.push_back(parse_atom(p_rule, "token"));
		alternative lexa(LEX);
		rule lexr("lexer");
		lexr.alternatives.push_back(lexa);
		pg.rules.push_back(lexr);
		
		// token
		parse_group TOK1;
		TOK1.atoms.push_back(parse_atom(p_rule, "type"));
		TOK1.atoms.push_back(parse_atom(p_token, "ID"));
		parse_group TOK2; TOK2.type = g_repeat;
		TOK2.atoms.push_back(parse_atom(p_rule, "lexer_group"));
		parse_group TOK3;
		TOK3.atoms.push_back(parse_atom(p_token, "SEMICOLON", true));
		alternative toka(TOK1);
		toka.groups.push_back(TOK2);
		toka.groups.push_back(TOK3);
		rule tokr("token");
		tokr.alternatives.push_back(toka);
		pg.rules.push_back(tokr);
		
		// lexer_group
		parse_group LEXGR1A; parse_group LEXGR1B;
		LEXGR1A.atoms.push_back(parse_atom(p_token, "LP", true));
		parse_group LEXGR1AA;
		LEXGR1AA.atoms.push_back(parse_atom(p_rule, "lexer_atom"));
		parse_group LEXGR1AAA; LEXGR1AAA.type = g_repeat;
		LEXGR1AAA.atoms.push_back(parse_atom(p_token, "SEPARATOR", true));
		LEXGR1AAA.atoms.push_back(parse_atom(p_rule, "lexer_atom"));
		LEXGR1B.atoms.push_back(parse_atom(p_token, "RP", true));
		parse_group LEXGR2; LEXGR2.type = g_optional;
		LEXGR2.atoms.push_back(parse_atom(p_token, "MOD"));
		alternative lexgra(LEXGR1A);
		lexgra.groups.push_back(LEXGR1AA);
		lexgra.groups.push_back(LEXGR1AAA);
		lexgra.groups.push_back(LEXGR1B);
		lexgra.groups.push_back(LEXGR2);
		rule lexgrr("lexer_group");
		lexgrr.alternatives.push_back(lexgra);
		pg.rules.push_back(lexgrr);
		
		// type
		parse_group TYPT;
		TYPT.atoms.push_back(parse_atom(p_token, "TOKEN"));
		alternative typta(TYPT);
		parse_group TYPS;
		TYPS.atoms.push_back(parse_atom(p_token, "SKIP"));
		alternative typsa(TYPS);
		parse_group TYPP;
		TYPP.atoms.push_back(parse_atom(p_token, "PRIV"));
		alternative typpa(TYPP);
		rule typr("type");
		typr.alternatives.push_back(typta);
		typr.alternatives.push_back(typsa);
		typr.alternatives.push_back(typpa);
		pg.rules.push_back(typr);
		
		// lexer_atom
		parse_group LEXAT1A; LEXAT1A.type = g_optional;
		LEXAT1A.atoms.push_back(parse_atom(p_token, "REV"));
		parse_group LEXAT1;
		LEXAT1.atoms.push_back(parse_atom(p_token, "ID"));
		alternative lexat1a(LEXAT1A);
		lexat1a.groups.push_back(LEXAT1);
		
		parse_group LEXAT2A; LEXAT2A.type = g_optional;
		LEXAT2A.atoms.push_back(parse_atom(p_token, "REV"));
		parse_group LEXAT2;
		LEXAT2.atoms.push_back(parse_atom(p_token, "STRING"));
		alternative lexat2a(LEXAT2A);
		lexat2a.groups.push_back(LEXAT2);
		
		parse_group LEXAT4A; LEXAT4A.type = g_optional;
		LEXAT4A.atoms.push_back(parse_atom(p_token, "REV"));
		parse_group LEXAT4;
		LEXAT4.atoms.push_back(parse_atom(p_token, "LETTER"));
		LEXAT4.atoms.push_back(parse_atom(p_token, "DOUBLEDOT", true));
		LEXAT4.atoms.push_back(parse_atom(p_token, "LETTER"));
		alternative lexat4a(LEXAT4A);
		lexat4a.groups.push_back(LEXAT4);
		
		parse_group LEXAT3A; LEXAT3A.type = g_optional;
		LEXAT3A.atoms.push_back(parse_atom(p_token, "REV"));
		parse_group LEXAT3;
		LEXAT3.atoms.push_back(parse_atom(p_token, "LETTER"));
		alternative lexat3a(LEXAT3A);
		lexat3a.groups.push_back(LEXAT3);
		
		rule lexatr("lexer_atom");
		lexatr.alternatives.push_back(lexat1a);
		lexatr.alternatives.push_back(lexat2a);
		lexatr.alternatives.push_back(lexat4a);
		lexatr.alternatives.push_back(lexat3a);
		pg.rules.push_back(lexatr);
		
		// parser
		parse_group PAR;
		PAR.type = g_repeat;
		PAR.atoms.push_back(parse_atom(p_rule, "rule"));
		alternative para(PAR);
		rule parr("parser");
		parr.alternatives.push_back(para);
		pg.rules.push_back(parr);
		
		// rule
		parse_group RULE1;
		RULE1.atoms.push_back(parse_atom(p_token, "RULE", true));
		RULE1.atoms.push_back(parse_atom(p_token, "ID"));
		parse_group RULE2;
		RULE2.atoms.push_back(parse_atom(p_rule, "alternative"));
		parse_group RULE3; RULE3.type = g_repeat;
		RULE3.atoms.push_back(parse_atom(p_token, "SEPARATOR", true));
		RULE3.atoms.push_back(parse_atom(p_rule, "alternative"));
		parse_group RULE4;
		RULE4.atoms.push_back(parse_atom(p_token, "SEMICOLON", true));
		alternative rulea(RULE1);
		rulea.groups.push_back(RULE2);
		rulea.groups.push_back(RULE3);
		rulea.groups.push_back(RULE4);
		rule ruler("rule");
		ruler.alternatives.push_back(rulea);
		pg.rules.push_back(ruler);
		
		// alternative
		parse_group ALT1; ALT1.type = g_repeat;
		ALT1.atoms.push_back(parse_atom(p_rule, "parser_group", true));
		alternative alta(ALT1);
		rule altr("alternative");
		altr.alternatives.push_back(alta);
		pg.rules.push_back(altr);
		
		// parser group
		parse_group PARGR1A; parse_group PARGR1B;
		PARGR1A.atoms.push_back(parse_atom(p_token, "LP", true));
		parse_group PARGR1AA;
		PARGR1AA.atoms.push_back(parse_atom(p_rule, "parser_atom"));
		parse_group PARGR1AAA; PARGR1AAA.type = g_repeat;
		PARGR1AAA.atoms.push_back(parse_atom(p_rule, "parser_atom"));
		PARGR1B.atoms.push_back(parse_atom(p_token, "RP", true));
		parse_group PARGR2; PARGR2.type = g_optional;
		PARGR2.atoms.push_back(parse_atom(p_token, "MOD"));
		alternative pargra(PARGR1A);
		pargra.groups.push_back(PARGR1AA);
		pargra.groups.push_back(PARGR1AAA);
		pargra.groups.push_back(PARGR1B);
		pargra.groups.push_back(PARGR2);
		rule pargrr("parser_group");
		pargrr.alternatives.push_back(pargra);
		pg.rules.push_back(pargrr);
		
		// parser_atom
		parse_group PAT1; PAT1.type = g_optional;
		PAT1.atoms.push_back(parse_atom(p_token, "ASTMOD"));
		parse_group PAT2;
		PAT2.atoms.push_back(parse_atom(p_token, "ID"));
		rule patr("parser_atom");
		alternative patra;
		patra.groups.push_back(PAT1);
		patra.groups.push_back(PAT2);
		patr.alternatives.push_back(patra);
		pg.rules.push_back(patr);
	}
	
	pg.gen();
	
}



