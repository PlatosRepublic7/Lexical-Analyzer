// Testing file containing main function
#include "lex.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;


void print_vec(vector<string> inV, int lineNum) {
	cout << "Contents of File:" << endl;

	for (int i = 0; i < inV.size(); i++) {
		cout << inV[i] << endl;
	}
}


int main(int argc, char* argv[]) {
	string fName = "C:/TestFiles/Lex/prog1.txt", buffer = "";
	char inC;
	int lineNum = 1, numTokens = 0;
	ifstream inF;
	vector<string> fVec;
	LexItem token;
	vector<LexItem> lexVec;
	bool v_flag = false, int_flag = false, real_flag = false, str_flag = false, ident_flag = false;
	bool error_flag = false;

	if (argc == 1) {
		cerr << "NO SPECIFIED INPUT FILE FOUND" << endl;
		exit(1);
	}
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-v") {
			v_flag = true;
		}
		else if (arg == "-intconst") {
			int_flag = true;
		}
		else if (arg == "-fconst") {
			real_flag = true;
		}
		else if (arg == "-strconst") {
			str_flag = true;
		}
		else if (arg == "-ident") {
			ident_flag = true;
		}
		else if (arg[0] == '-') {
			cerr << "UNRECOGNIZED FLAG " << arg << endl;
			exit(1);
		}
		else {
			inF.open(arg);
			if (inF.is_open() == false) {
				cerr << "CANNOT OPEN THE FILE " << arg << endl;
				exit(1);
			}
		}

	}

	if (v_flag) {
		while ((token = getNextToken(inF, lineNum)) != DONE && token != ERR) {
			cout << token << endl;
		}
	}


	if (token.GetToken() == ERR) {
		cerr << "Error in line " << token.GetLinenum() << " (" << token.GetLexeme() << ")" << endl;
		exit(1);
	}
	else if (token.GetToken() == DONE) {
		cout << "Lines: " << endl;
		cout << "Tokens: " << endl;
	}

	inF.close();

/*
	inF.open(fName);
	while (!inF.eof()) {
		inF.get(inC);
		if (inC == '\n') {
			lineNum++;
			fVec.push_back(buffer);
			buffer = "";
		}
		else
			buffer += inC;
	}
	inF.close();

	print_vec(fVec, lineNum);
	lineNum = 1;
	cout << endl << endl;

	inF.open(fName);
	while ((tok = getNextToken(inF, lineNum)) != DONE && tok != ERR) {
		if (tok.GetLinenum() != 0) {
			lexVec.push_back(tok);
			numTokens++;
		}
	}
	inF.close();

	cout << endl << "Lines: " << lineNum - 1 << endl;
	cout << "Tokens: " << lexVec.size() << endl << endl;

	for (int i = 0; i < lexVec.size(); i++) {
		cout << "Token: " << lexVec[i] << " Lexeme: " << lexVec[i].GetLexeme() << " Line: " << lexVec[i].GetLinenum() << endl;
	}
*/
	return 0;
}