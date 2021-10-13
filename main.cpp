// Testing file containing main function
#include "lex.h"
#include <iostream>
#include <fstream>
#include <vector>

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
	LexItem tok;
	vector<LexItem> lexVec;

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
	return 0;
}