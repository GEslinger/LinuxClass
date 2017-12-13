#include <string>
#include <regex>

const string numEx = "^-?\\d{1,4}$";
const string lineEx = "^\\d{1,2}$";
const string symEx = "^[a-z]|[A-Z]$";

bool checkNum(string s){
	return regex_match(s, regex(numEx));
}
bool checkLine(string s){
	return regex_match(s, regex(lineEx));
}
bool checkSym(string s){
	return regex_match(s, regex(symEx));
}

int getRelopFunction(string s){
	if(s == ">"){
		return 1;
	}
	else if(s == ">="){
		return 2;
	}
	else if(s == "<"){
		return 3;
	}
	else if(s == "<="){
		return 4;
	}
	else if(s == "=="){
		return 5;
	}
	else if(s == "!="){
		return 6;
	} else {
		return -1;
	}
}