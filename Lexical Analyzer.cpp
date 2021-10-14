#include "lex.h"
#include <cctype>


map<string, Token> ops = {
	{ "+", PLUS}, {"-", MINUS}, {"*", MULT}, {"/", DIV}, {"%", REM}, {"(", LPAREN}, {")", RPAREN}, {",", COMMA}, {"=", EQUAL}, {">", GTHAN}, {";", SEMICOL},
};

map<string, Token> tok_str_map = {
	{"PLUS", PLUS}, {"MINUS", MINUS}, {"MULT", MULT}, {"DIV", DIV}, {"REM", REM}, {"LPAREN", LPAREN}, {"RPAREN", RPAREN}, {"COMMA", COMMA}, {"EQUAL", EQUAL},
	{"GTHAN", GTHAN}, {"SEMICOL", SEMICOL}, {"PROGRAM", PROGRAM}, {"END", END}, {"BEGIN", BEGIN}, {"WRITE", WRITE}, {"IF", IF}, {"INT", INT}, {"FLOAT", FLOAT},
	{"STRING", STRING}, {"REPEAT", REPEAT}
};

LexItem getNextToken(istream& in, int& linenum) {
	enum Tokstate { START, INID, INSTRING, ININT, INREAL, INCOMMENT, INOPERATOR };
	Tokstate lexstate = START;
	string lexeme;
	LexItem lexitem = LexItem();
	bool is_op = false;
	char ch;
/*	map<string, Token> tokMap = {
		{ "+", PLUS}, {"-", MINUS}, {"*", MULT}, {"/", DIV}, {"%", REM}, {"(", LPAREN}, {")", RPAREN}, {",", COMMA}, {"=", EQUAL}, {">", GTHAN}, {";", SEMICOL},
		{"IDENT", IDENT}, {"ICONST", ICONST}, {"RCONST", RCONST}, {"SCONST", SCONST}, {"program", PROGRAM}, {"end", END}, {"begin", BEGIN}, {"write", WRITE}, 
		{"if", IF}, {"int", INT}, {"float", FLOAT}, {"string", STRING}, {"repeat", REPEAT} 
	};
*/
	while (in.get(ch)) {
		switch (lexstate) {
		case START:
			if (ch == '\n')
				linenum++;
			if (isspace(ch))
				continue;
			else if (isalpha(ch)) {
				lexeme += ch;
				lexstate = INID;
			}
			else if (isdigit(ch)) {
				lexeme += ch;
				lexstate = ININT;
			}
			else if (ch == '.') {
				lexeme += ch;
				lexstate = INREAL;
			}
			else if (ch == '"') {
				lexstate = INSTRING;
			}
			else if (ch == '#')
				lexstate = INCOMMENT;
			else if (ispunct(ch)) {
				in.putback(ch);
				lexstate = INOPERATOR;
			}
			break;

		case INID:
			in.putback(ch);
			while (in.get(ch)) {
				if (ch == '\n') {
					lexitem = id_or_kw(lexeme, linenum);
					linenum++;
					return lexitem;
				}
				else if (isspace(ch) || ispunct(ch)) {
					lexitem = id_or_kw(lexeme, linenum);
					in.putback(ch);
					return lexitem;
				}
				else
					lexeme += ch;
			}

			break;

		case INSTRING:
			in.putback(ch);
			while (in.get(ch)) {
				if (ch == '"') {
					lexitem = LexItem(SCONST, lexeme, linenum);
					return lexitem;
				}
				else if (ch == '\n') {
					lexitem = LexItem(ERR, lexeme, linenum);
					return lexitem;
				}
				else
					lexeme += ch;
			}

			break;

		case ININT:
			in.putback(ch);
			while (in.get(ch)) {
				if (ch == '.') {
					lexstate = INREAL;
					lexeme += ch;
					break;
				}
				else if (ch == '\n') {
					lexitem = LexItem(ICONST, lexeme, linenum);
					linenum++;
					return lexitem;
				}
				else if (isspace(ch)) {
					lexitem = LexItem(ICONST, lexeme, linenum);
					return lexitem;
				}
				else if (ispunct(ch) || isalpha(ch)) {
					if (ch == ';' || ch == ')') {
						lexitem = LexItem(ICONST, lexeme, linenum);
						in.putback(ch);
					}
					else {
						lexitem = LexItem(ERR, lexeme, linenum);
						in.putback(ch);
					}
					return lexitem;
				}
				else
					lexeme += ch;
			}

			break;

		case INREAL:
			if (!isdigit(ch)) {
				lexitem = LexItem(ERR, lexeme, linenum);
				return lexitem;
			}

			in.putback(ch);
			while (in.get(ch)) {
				if (ch == '\n') {
					lexitem = LexItem(RCONST, lexeme, linenum);
					linenum++;
					return lexitem;
				}
				else if (isspace(ch)) {
					lexitem = LexItem(RCONST, lexeme, linenum);
					return lexitem;
				}
				else if (ispunct(ch) || isalpha(ch)) {
					if (ch == ';' || ch == ')') {
						lexitem = LexItem(RCONST, lexeme, linenum);
						in.putback(ch);
					}
					else {
						lexitem = LexItem(ERR, lexeme, linenum);
						in.putback(ch);
					}
					return lexitem;
				}
				else
					lexeme += ch;
			}

			break;

		case INCOMMENT:
			while (ch != '\n') {
				in.get(ch);
			}
			linenum++;
			lexstate = START;

			break;

		case INOPERATOR:
			lexeme += ch;

			if (lexeme == "=" && in.peek() == '=') {
				in.get(ch);
				lexeme += ch;
				lexitem = LexItem(ASSOP, lexeme, linenum);
				is_op = true;
			}
			else {
				for (auto it = ops.begin(); it != ops.end(); it++) {
					if (lexeme == it->first) {
						lexitem = LexItem(it->second, lexeme, linenum);
						is_op = true;
						break;
					}
				}
			}

			if (!is_op) {
				lexitem = LexItem(ERR, lexeme, linenum);
				return lexitem;
			}
			else
				return lexitem;

			break;
		}
	}
	if (in.eof()) {
		lexitem = LexItem(DONE, lexeme, linenum);
		return lexitem;
	}
}


LexItem id_or_kw(const string& lexeme, int linenum) {
	map<string, Token> keywords = { 
		{"program", PROGRAM}, {"PROGRAM", PROGRAM}, {"END", END}, {"end", END}, {"BEGIN", BEGIN}, {"begin", BEGIN}, {"WRITE", WRITE}, {"write", WRITE}, 
		{"IF", IF},  {"if", IF}, {"INT", INT}, {"int", INT}, {"FLOAT", FLOAT}, {"float", FLOAT}, {"STRING", STRING}, {"string", STRING}, {"REPEAT", REPEAT}, {"repeat", REPEAT}
	};
	bool notFound = true;
	Token t;

	for (auto it = keywords.begin(); it != keywords.end(); it++) {
		if (lexeme == it->first) {
			t = it->second;
			notFound = false;
		}
	}

	if (notFound)
		t = IDENT;

	LexItem lexitem = LexItem(t, lexeme, linenum);

	return lexitem;
}


ostream& operator<<(ostream& out, const LexItem& tok) {
	map<string, Token> identMap = { {"IDENT", IDENT}, {"ICONST", ICONST}, {"RCONST", RCONST}, {"SCONST", SCONST} };
	string token;
	for (auto it = identMap.begin(); it != identMap.end(); it++) {
		if (tok.GetToken() == it->second) {
			token = it->first;
			out << token << " (" << tok.GetLexeme() << ")";
			return out;
		}
	}
	
	for (auto it = tok_str_map.begin(); it != tok_str_map.end(); it++) {
		if (tok.GetToken() == it->second) {
			token = it->first;
			out << token;
			return out;
		}
	}
}