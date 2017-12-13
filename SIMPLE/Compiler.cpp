#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <queue>
#define BEGIN_PROGRAM
#include "SMLRunner.cpp"
#include "AssertionTools.cpp"
using namespace std;

int CL = 0;		// Current line in the code
int CMD = 0;	// Pointer to the current command in the SML
int DAT = 99;	// Pointer to the data in SML

bool ENDED = false;		// Has the code hit an end instruction?
map<string,int> SYMBOL;	// Symbol table (tracks values stored as data: variables and literals) mapped to their position in SML
map<int,int> LINENUM;	// Table of line numbers in SIMPLE to commands in SML

map<int,int> UNRESOLVED;// Table of unresolved line numbers to the positions in SML to modify

SMLRunner CODE;	// The SML object


void throwError(int type, int lineNum, string dat){	// Function that throws errors and notifies the user
	cerr << "ERROR:\n";
	switch(type){									// Include options for different types of error
		case 1: cerr << "Assertion failed: " << dat; break;
		case 2: cerr << "Symbol " << dat << " not found in symbol table"; break;
		case 3: cerr << "Unexpected instruction " << dat; break;
		case 4: cerr << "No end instruction"; break;
		case 5: cerr << "Unresolved line number " << dat << endl; exit(1); break;
		default: cerr << "Something went terribly wrong"; break;
	}
	cerr << " at " << lineNum << endl;	// Tell the user where the error occurs
	exit(1);							// Kill the program
}

void checkCollision(){
	if(CMD >= DAT) throwError(1,CL,"Command pointer is lower than data pointer");
}

void assertNum(string s){	// Assert that a string is a number, in terms of SML (int x between -9999 and 9999) using a regex
	if(!checkNum(s)){
		throwError(1,CL,"Number expected");	// If it's not a number, throw an error
	}
}

void assertLine(string s){	// Assert that a string is a line number (0 to 99)
	if(!checkLine(s)){
		throwError(1,CL,"Next line number expected");
	}
}

void assertSym(string s){	// Assert that a string is a symbol (a - z)
	if(!checkSym(s)){
		throwError(1,CL,"Symbol name expected");
	}
}

void assertWord(string s, string expected){	// Assert that a string is equal to an expected string
	if(s != expected){
		throwError(3,CL,s);
	}
}

vector<string> inputFile(string fname){	// Input a file!
	ifstream inFile;	// Define a file stream reader
	inFile.open(fname);	// Open the specified file
	if(inFile.fail()){	// If no worky, then err0r
		cerr << "Unable to open file\n";
		exit(1);
	}

	string in;			// Define a string to hold the line
	vector<string> out;	// The vector of strings (lines) to output
	while(getline(inFile,in)){	// Get a line from the stream
		out.push_back(in);		// Add it to the ouput
	}

	return out;
}

vector<string> getTerms(string line){	// Terms (strings of characters/numbers separated by spaces)
	stringstream ss(line);				// Create a string stream to read the line out of
	istream_iterator<string> begin(ss);	// Iterators from the beginning to the end
	istream_iterator<string> end;
	vector<string> words(begin,end);	// Read / split the string into a new vector

	return words;
}

void attemptLiteral(string s){	// Attempt to parse a string into a number, add to symbol table
	assertNum(s);				// Assert that it's a number (so we can safely run stoi)

	SYMBOL.insert(make_pair(s,DAT));	// Insert it into the symbol table
	CODE.setWord(DAT,stoi(s));			// Add the data into the SML code at the data pointer (DAT must be decremented sometime after running this function!)
}

void assertTerms(vector<string> terms, int lower, int higher){	// Assert that a line has a number of terms between a lower and upper bound
	if(terms.size() < lower || terms.size() > higher) throwError(1,CL,"Invalid number of terms");
}




int getPrec(string s){			// The precedence of operators for use in parsing infix to postfix
	if(s == "+" || s == "-"){	// If we wanted to add an operator of even higher precedence that * and / (such as ^), it would return -2, then -3, etc.
		return 0;
	} else if(s == "*" || s == "/"){
		return -1;
	} else if(s == "("){
		return 1;
	} else if(s == ")"){
		return 2;
	} else {
		return 3;
	}
}

int getOp(string s){	// Get the SML operation of the corresponding symbol
	if(s == "+"){
		return 3000;
	} else if (s == "-"){
		return 3100;
	} else if (s == "*"){
		return 3300;
	} else if (s == "/"){
		return 3200;
	} else {
		return 0;
	}
}

int getRelOp(string s){	// Relational Operators
	if(s == "=="){
		return 1;
	} else if (s == "<="){
		return 2;
	} else if (s == ">="){
		return 3;
	} else if (s == "<"){
		return 4;
	} else if (s == ">"){
		return 5;
	} else if (s == "!="){
		return 6;
	}

	return 0;
}


int getValueAddress(string expr){	// Get the SML address of a string (that may or may not be in the symbol table)
	auto pos = SYMBOL.find(expr);	// Create a map iterator at the position of the string
	if(pos != SYMBOL.end()){		// If the iterator isn't at the end, that means it found the value
		return pos->second;			// Return the address
	} else {
		throwError(2,CL,expr);		// Otherwise, the symbol wasn't found, throw an error
	}
	return -1;	// By default (this should never happen) it returns -1 to hopefully cause an error later on
}


void loadExpr(vector<string> inFix){	// Generates the SML code to evaluate an expression and put the output in the accumulator.
	queue<string> postFix;	// Queue of terms to hold the postfix after conversion

	stack<string> ops;		// Stack of operators


	int parens = 0;	// Track the number of parenthesis that we are within

	for(int i = 0; i < inFix.size(); i++){
		string s = inFix[i];	// Get each symbol in the expression
		int op = getPrec(s);	// Attempt to get its precedence as an operator (it's 3 if it's not an operator)

		if(parens < 0) throwError(1,CL,"Matching Parentheses");	// If parens is negative, there have been too many closing parentheses, so error

		if(checkNum(s)){		// If the symbol matches as a number,
			attemptLiteral(s);	// Try adding it as a literal
			DAT--;
			postFix.push(s);	// Add it to stack
		}
		else if(checkSym(s)){	// If it's a symbol, push on stack
			postFix.push(s);
		}
		else if(op <= 0){		// Handle operators and precedence
				while(!ops.empty() && getPrec(ops.top()) <= op){
					postFix.push(ops.top());
					ops.pop();
				}
			ops.push(s);
		}
		else if(op == 1){
			ops.push(s);
			parens++;
		}
		else if(op == 2){
			while(!ops.empty() && getPrec(ops.top()) != 1){
				postFix.push(ops.top());
				ops.pop();
			}
			ops.pop();
			parens--;
		}
		else{
			//cout << "Aaa";
			throwError(1,CL,"Expression contains only valid numbers and operators");
		}
	}

	while(!ops.empty()){	// Empty leftover operators onto the postfix
		postFix.push(ops.top());
		ops.pop();
	}


	if(parens != 0) throwError(1,CL,"Matching Parentheses");	// If parentheses haven't been closed, explode

	stack<int> operands;

	while(!postFix.empty()){				// Evaluate postfix fun things, though instead of performing operations, add code to perform them to the SML
		string s = postFix.front();			// Algorithm stolen from http://www.geeksforgeeks.org/stack-set-4-evaluation-postfix-expression/

		if(checkSym(s) || checkNum(s)){
			operands.push(getValueAddress(s));
		}
		else if(getOp(s)){
			if(operands.size() < 2){
				throwError(1,CL,"Expression has operands for all operators");
			}

			int second = operands.top();
			operands.pop();
			CODE.setWord(CMD++,2000+operands.top());
			operands.pop();
			CODE.setWord(CMD++,getOp(s)+second);
			CODE.setWord(CMD++,2100+DAT);
			operands.push(DAT);
			DAT--;
		}
		else{
			throwError(1,CL,"Expression contains only valid numbers and operators");
		}
		postFix.pop();

		checkCollision();
	}

	if(!operands.empty()){							// If there's an operand left over, just load it
		CODE.setWord(CMD++,2000+operands.top());	// (to deal with an expression of a single symbol)
		operands.pop();
	}
	if(!operands.empty()){	// If there are any more operands left over, throw an error.
		throwError(1,CL,"Expression has operands for all operators");	
	}
}




void SML_INPUT(vector<string> terms){		// Function called to add input command to SML
	string var = terms[2];					// Var is the name of the variable to input

	auto pos = SYMBOL.find(var);			// Create an iterator through the symbol table to find var
	if(pos != SYMBOL.end()){				// If var is found,
		CODE.setWord(CMD,1000+pos->second);	// Add a command that saves the input in var's address
	} else {
		CODE.setWord(CMD,1000+DAT);			// Otherwise, add a command that saves the input to the next available data pointer
		SYMBOL.insert(make_pair(var,DAT--));// Add an entry in the data table and decrement the data pointer
	}

	CMD++;	// Increment command pointer
}

void SML_PRINT(vector<string> terms){	// Add print command to SML

	auto pos = SYMBOL.find(terms[2]);	// Try to find the specified variable
	if(pos != SYMBOL.end()){
		CODE.setWord(CMD,1100+pos->second);	// If it's found, add command to print it
	} else {
		throwError(2,CL,terms[2]);		// Otherwise, ERR0R
	}

	CMD++;	// Increment
}

void SML_END(){
	CODE.setWord(CMD++,4300);	// Add end instruction
	ENDED = true;				// Let the program know there has been at least one end instruction (but not that it's reachable!)
}

void SML_GOTO(vector<string> terms){		// Goto command
	int n = stoi(terms[2]);					// Convert the address into an int (this is safe because of an earlier assertion)

	auto pos = LINENUM.find(n);				// Try to find the corresponding line number in the LINENUM table
	if(pos != LINENUM.end()){				// If it exists,
		CODE.setWord(CMD,4000+pos->second);	// Add a hard jump command to that pos
	} else {
		CODE.setWord(CMD,4000);				// If it doesn't, add a generic hard jump command (to be modified later with the correct line number)
		UNRESOLVED.insert(make_pair(CMD,n));// And add the command pointer and unresolved line number to a map of unresolved linenums. 
	}										// (In this order because the line number can be referenced by multiple cmd positions)
	CMD++;
}

void SML_IF(vector<string> terms){			// IF command! Scary~~~
	int firstID = getValueAddress(terms[2]);// Try to get the address of the first ID (left hand side of relop is just an ID, not expr (it says so in the specification (hello)))

	int op = getRelOp(terms[3]);			// Get the value of the relational operator

	if(!op) throwError(1, CL, "If statement contains relop");	// If there isn't one (getRelOp returns 0) then ERR0R

	
	vector<string> expr;										// Vector of space-separated, 1-char strings that holds the expression
	copy(terms.begin()+4, terms.end()-2, back_inserter(expr));	// Copy the terms from the 4th (after the relop) to the 2nd away from the end (before the goto)
	
	int line = stoi(terms.back());	// Turn the final term (the line number of the goto) into an int
	auto pos = LINENUM.find(line);	// Try to find the line number
	

	if(op == 1 || op == 3 || op == 5 || op == 6){	// If the relop is one where the expression should be loaded first
		loadExpr(expr);						// Load it
		CODE.setWord(CMD++,3100+firstID);	// Then subtract the stuff from the first ID
	} 

	if(op == 2 || op == 4){					// Otherwise,
		loadExpr(expr);						// Load the expression
		CODE.setWord(CMD++,2100+DAT);		// Save the result??
		CODE.setWord(CMD++,2000+firstID);	// Load the first ID
		CODE.setWord(CMD++,3100+DAT);		// Subtract the expression
		DAT--;	// Decrement
	}

	if(op == 2 || op == 3 || op == 4 || op == 5){	// For relops that use > or <
		if(pos != LINENUM.end()){
			CODE.setWord(CMD,4100+pos->second);		// Use 4100 as a conditional jump if the acc is negative
		} else {
			CODE.setWord(CMD,4100);
			UNRESOLVED.insert(make_pair(CMD,line));
		}
		CMD++;
	}

	if(op == 1 || op == 3 || op == 2){				// For things that use = (but not !=)
		if(pos != LINENUM.end()){
			CODE.setWord(CMD,4200+pos->second);		// Also use 4200 to check if the acc is 0
		} else {
			CODE.setWord(CMD,4200);
			UNRESOLVED.insert(make_pair(CMD,line));
		}
		CMD++;
	}

	if(op == 6){								// For the crazy != relop
		CODE.setWord(CMD++,4200+CMD+2);			// Jump to the next command if it's equal

		if(pos != LINENUM.end()){
			CODE.setWord(CMD,4000+pos->second);	// If the program passes over the jump if equal, then it's not equal, so jump to the specified location
		} else {
			CODE.setWord(CMD,4000);
			UNRESOLVED.insert(make_pair(CMD,line));
		}
		CMD++;
	}
}

void SML_LET(vector<string> terms){		// Let command

	vector<string> expr;
	copy(terms.begin()+4, terms.end(), back_inserter(expr));	// Copy the expression into a vector

	loadExpr(expr);	// Load it into the accumulator

	string var = terms[2];
	auto pos = SYMBOL.find(var);
	if(pos != SYMBOL.end()){
		CODE.setWord(CMD,2100+pos->second);	// Set the corresponding variable to the value
	} else {
		CODE.setWord(CMD,2100+DAT);
		SYMBOL.insert(make_pair(var,DAT));	// Or create a new variable with that value, add to symbol table
		DAT--;
	}
	CMD++;
}



template<typename A, typename B>
void printMap(map<A,B> thing){
	for(typename map<A,B>::iterator i = thing.begin(); i != thing.end(); i++){	// Useful Debug function
		cout << i->first << " => " << i->second << endl;
	}
}


void runCmd(vector<string> terms){	// Main command runner, asserts things about the command to make the SML_ functions safer
	if(terms[1] == "rem"){
		return;
	}
	else if(terms[1] == "input"){
		assertTerms(terms,3,3);
		assertSym(terms[2]);
		SML_INPUT(terms);
	}
	else if(terms[1] == "print"){
		assertTerms(terms,3,3);
		assertSym(terms[2]);
		SML_PRINT(terms);
	}
	else if(terms[1] == "end"){
		assertTerms(terms,2,2);
		SML_END();
	}
	else if(terms[1] == "goto"){
		assertTerms(terms,3,3);
		assertLine(terms[2]);
		SML_GOTO(terms);
	}
	else if(terms[1] == "if"){
		assertTerms(terms,7,99);
		assertSym(terms[2]);
		assertLine(terms.back());
		SML_IF(terms);
	}
	else if(terms[1] == "let"){
		assertSym(terms[2]);
		assertWord(terms[3], "=");
		SML_LET(terms);
	}
	else{
		throwError(3,CL,terms[1]);
	}
}


int main(){			// Main loop
	vector<string> lines = inputFile("hello.txt");	// Input the file


	for(int i = 0; i < lines.size(); i++){			// First pass
		vector<string> terms = getTerms(lines[i]);	// Get each word into a vector
		assertLine(terms[0]);				// Assert that the first word is a line number
		int ln = stoi(terms[0]);			// Convert
		LINENUM.insert(make_pair(ln,CMD));	// Insert line number into the LINENUM table
		CL = ln;		// Save the current line number into a global variable

		runCmd(terms);	// Run the command on that line

		checkCollision();	// Make sure it's not destroying itself
	}

	if(!ENDED) throwError(4,CL,"");	// If the code hasn't had an end instruction, explode


	for(map<int,int>::iterator i = UNRESOLVED.begin(); i != UNRESOLVED.end(); i++){	// Second pass, iterating through unresolved line numers
		int line = i->second;				// The line number being referenced

		auto jmpval = LINENUM.find(line);	// Try to find it in the LINENUM table now that all lines have been processed
		if(jmpval != LINENUM.end()){		// If it exists,
			CODE.addWord(i->first,jmpval->second);		// Add the the line number's address to the unresolved command 
		} else {
			throwError(5,-1,to_string(line));	// Otherwise, thow an error, line number not found
		}
	}

	cout << "Success. Saving code as main.sml...\n";

	CODE.saveCode("main.sml");

	// CODE.memDump();

	cout << "Running...\n";
	
	CODE.exec();	// Execute the code

	return 0;
}