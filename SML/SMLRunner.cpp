#include <iostream>
#include <fstream>
#include "SMLRunner.h"

SMLRunner::SMLRunner(){
	ACC = 0;
	CMD = 0;
	for(int i = 0; i < 100; i++){
		MEM[i] = 0;
	}
}

void SMLRunner::reset(){
	ACC = 0;
	CMD = 0;
	for(int i = 0; i < 100; i++){
		MEM[i] = 0;
	}
}

bool SMLRunner::isWord(int w) const {
	return (w <= 9999) && (w >= -9999);
}

int SMLRunner::getCommand(int w) const {
	return w/100;
}

int SMLRunner::getAddr(int w) const {
	return w%100;
}

bool SMLRunner::loadFile(string fname){
	ifstream inFile;
	inFile.open(fname);
	if(inFile.fail()){
		cerr << "Unable to open file\n";
		return false;
	}

	int addr = 0;
	int w = 0;
	while(inFile >> w){
		if(!isWord(w)){
			cerr << "Non-word data detected in file\n";
			return false;
		}

		MEM[addr] = w;

		if(addr++ > 99){
			cerr << "File too big\n";
			return false;
		}
	}
}

void SMLRunner::handleInput(int address){
	int maybe;
	cout << "IN: ";
	cin >> maybe;
	MEM[address] = maybe;
}

void SMLRunner::overflow(int address){
	if(MEM[address] > 9999 || MEM[address] < -9999){
		MEM[address] = MEM[address] % 10000;
		cerr << "OVERFLOW AT ADDRESS " << address << "; WRAPPING\n";
	}
}

void SMLRunner::memDump(){
	cerr << "MEMDUMP CALLED AT POS " << CMD << endl;
	for(int i = 0; i < 100; i++){
		if(i%10 == 0){
			cerr << endl;
		}

		cerr << MEM[i] << "\t";
	}
	cerr << endl;
}

bool SMLRunner::exec(){
	bool done = false;

	while(!done){
		int w = MEM[CMD];
		int c = getCommand(w);
		int d = getAddr(w);
		overflow(d);

		switch(c){
			case 10: handleInput(d); break;
			case 11: cout << "OUT: " << MEM[d] << endl; break;
			case 20: ACC = MEM[d]; break;
			case 21: MEM[d] = ACC; break;
			case 30: ACC += MEM[d]; break;
			case 31: ACC -= MEM[d]; break;
			case 32: ACC /= MEM[d]; break;
			case 33: ACC *= MEM[d]; break;
			case 40: CMD = d; continue;
			case 41: if(ACC < 0){ CMD = d; continue; } break;
			case 42: if(ACC == 0){ CMD = d; continue; } break;
			case 43: done = true; break;
			case 99: memDump(); break;
			default: cerr << "BAD COMMAND\n"; memDump(); return false;
		}

		overflow(d);
		CMD++;
	}

	return true;
}


int main(){
	SMLRunner stoopid;
	stoopid.loadFile("avg.sml");
	stoopid.exec();

	return 0;
}