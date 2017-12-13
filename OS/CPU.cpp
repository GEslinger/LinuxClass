#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#define dtype unsigned short
#define ctype unsigned char
using namespace std;

class CPU{
public:
	CPU(ctype* memPtr);
	void tick();
	void dump();
	bool HLT = false;

private:
	void noArg(ctype cmd);
	void oneArgF(ctype cmd, ctype dat);
	void oneArgS(ctype cmd, ctype dat);
	void twoArg(ctype cmd, ctype dat);
	void runInt(dtype d);
	void doCMP(dtype f, dtype s);
	dtype getFirst(ctype dat);
	dtype getSecond(ctype dat, ctype offset);
	void setFirst(ctype dat, dtype val);

	void error(ctype cmd);

	ctype* mem;				// Pointer to the array for memory

	dtype AX = 0;	// The four general-use pointers (Split into top and bottom with AH, AL, etc.)
	dtype BX = 0;
	dtype CX = 0;
	dtype DX = 0;
	dtype FG = 0;	// Flags: Results of comparison, carry bit, etc
	dtype BP = 0;	// Stores the address of some position in the stack
	dtype SP = 0;	// Stores the address of the top of the stack
	
	dtype IP = 0;
	dtype NP = 0;
};

CPU::CPU(ctype* memPtr){
	mem = memPtr;
}

void CPU::dump(){
	cout << "AX\tBX\tCX\tDX\tFG\tBP\tSP\n";
	cout << hex << AX << "\t" << BX << "\t" << CX << "\t" << DX << "\t";
	cout << hex << FG << "\t" << BP << "\t" << SP << endl;
}

void CPU::error(ctype cmd){
	cerr << "\nINVALID INSTRUCTION: 0x" << hex << (int)cmd << " AT POS 0x" << hex << IP << endl;
	cerr << "DUMPING MEM TO error.out...";

	ofstream err;
	err.open("error.out");
	for(int i = 0; i < 65536; i++){
		err << hex << (int)mem[i] << " ";
	}
	err.close();

	cerr << " DONE\n";
	cerr << "\nDUMP SUMMARY:\n";
	dump();
	cerr << "ADDR / MEM\n";
	for(int i = IP-10; i < IP+10; i++){
		cerr << hex << i << "\t";
	}
	cerr << endl;
	for(int i = IP-10; i < IP+10; i++){
		cerr << hex << (int)mem[i] << "\t";
	}
	cerr << endl;
	cerr << "\t\t\t\t\t\t\t\t\t\t^^^^\n";

}

void CPU::runInt(dtype d){
	if(d == 0x1){
		string in;
		getline(cin,in);
		AX = in.length();

		for(int i = in.length()-1; i >= 0; i--){
			SP--;
			mem[SP] = (ctype)in.at(i);
		}
	}

	if(d == 0x2){
		cout << (ctype)(AX);
	}
}

void CPU::doCMP(dtype f, dtype s){
	dtype NF = 0;
	if(f > s){
		NF += 0x200;
	}
	if(f == s){
		NF += 0x400;
	}
	if(f < s){
		NF += 0x100;
	}

	NF += 0x2000*(f&0x01);
	NF += f&0x4000;
	NF += f&0x8000;

	if(f == 0){
		NF += 0x1000;
	}

	FG = NF;
}

dtype CPU::getFirst(ctype dat){
	ctype reg = dat >> 4;
	switch(reg){
		case 0x1: return AX >> 8;		// AH
		case 0x2: return BX >> 8;		// BH
		case 0x3: return CX >> 8;		// CH
		case 0x4: return DX >> 8;		// DH
		case 0x5: return AX & 0xFF;		// AH
		case 0x6: return BX & 0xFF;		// BH
		case 0x7: return CX & 0xFF;		// CH
		case 0x8: return DX & 0xFF;		// DH
		case 0x9: return AX;
		case 0xA: return BX;
		case 0xB: return CX;
		case 0xC: return DX;
		case 0xD: return FG;
		case 0xE: return BP;
		case 0xF: return SP;
	}

	dtype addr = (dtype)mem[IP+2]*0x100;	// If no register recognized, parse next two bytes as mem address
	addr += (dtype)mem[IP+3];
	
	return (dtype)mem[addr]*100+(dtype)mem[addr+1];
}


dtype CPU::getSecond(ctype dat, ctype offset){
	ctype extra = dat & 0x0F;
	dtype val = (dtype)mem[IP+offset]*0x100;
	val += (dtype)mem[IP+offset+1];

	switch(extra){
		case 0b0001: return getFirst(mem[IP+offset]);
		case 0b0010: return (dtype)mem[val]*100+(dtype)mem[val+1];
		case 0b0011: return (dtype)mem[BX]*100+(dtype)mem[BX+1];
		case 0b0100: return val;
		// default: cerr << "bad extra bits\n";// exit(1);
	}
}

void CPU::setFirst(ctype dat, dtype val){
	ctype reg = dat >> 4;
	// cout << hex << reg << endl;

	switch(reg){
		case 0x1: AX = (val << 8) + (AX&0xFF); return;		// AH
		case 0x2: BX = (val << 8) + (BX&0xFF); return;		// BH
		case 0x3: CX = (val << 8) + (CX&0xFF); return;		// CH
		case 0x4: DX = (val << 8) + (DX&0xFF); return;		// DH
		case 0x5: AX = (AX&0xFF00) + (val&0xFF); return;	// AH
		case 0x6: BX = (BX&0xFF00) + (val&0xFF); return;	// BH
		case 0x7: CX = (CX&0xFF00) + (val&0xFF); return;	// CH
		case 0x8: DX = (DX&0xFF00) + (val&0xFF); return;	// DH
		case 0x9: AX = val; return;
		case 0xA: BX = val; return;
		case 0xB: CX = val; return;
		case 0xC: DX = val; return;
		case 0xD: FG = val; return;
		case 0xE: BP = val; return;
		case 0xF: SP = val; return;
	}

	dtype addr = (dtype)mem[IP+2]*0x100;	// If no register recognized, parse next two bytes as mem address
	addr += (dtype)mem[IP+3];

	mem[addr] = (ctype)(val);
	mem[addr+1] = (ctype)(val >> 8);
}

void CPU::noArg(ctype cmd){
	switch(cmd){
		case 0x00: return;														// NOP
		case 0x01: cerr << "HALTED\n"; HLT = true; break;						// HLT
		case 0x02: IP = mem[SP]; SP++; break;									// RET
		case 0x03: CX++; break;													// INC
		case 0x04: CX--; break;													// DEC
		case 0x05: SP++; break;													// POP
		default: error(cmd);
	}

}

void CPU::oneArgF(ctype cmd, ctype dat){
	if(dat>>4 == 0){
		NP += 2;	// If there is no register, it should know there will be an address
	}

	dtype F = getFirst(dat);
	switch(cmd){
		case 0x10: setFirst(dat, F+(dtype)1); break;							// INC
		case 0x11: setFirst(dat, F-(dtype)1); break;							// DEC
		case 0x12: setFirst(dat, -F); break;									// NEG
		case 0x13: setFirst(dat, mem[SP]); SP++; break;							// POP
		case 0x14: setFirst(dat, !F);											// NOT
		default: error(cmd);
	}
}

void CPU::oneArgS(ctype cmd, ctype dat){
	if(dat>>4 == 0){
		NP += 2;
	}

	dtype F = getSecond(dat,2);
	switch(cmd){
		case 0x30: SP--; mem[SP] = F; break;									// PUSH
		case 0x31: NP = F; break;												// JMP
		case 0x32: if(FG&0x400) NP = F; break;									// JE
		case 0x33: if(!FG&0x400) NP = F; break;									// JNE
		case 0x34: if(FG&0x100) NP = F; break;									// JL
		case 0x35: if(FG&0x100 || FG&0x400) NP = F; break;						// JLE
		case 0x36: if(FG&0x200) NP = F; break;									// JG
		case 0x37: if(FG&0x200 || FG&0x400) NP = F; break;						// JGE
		case 0x38: if(FG&0x4000) NP = F; break;									// JO
		case 0x39: if(!FG&0x4000) NP = F; break;								// JNO
		case 0x3A: if(FG&0x8000) NP = F; break;									// JC
		case 0x3B: if(!FG&0x8000) NP = F; break;								// JNC
		case 0x3C: if(FG&0x2000) NP = F; break;									// JP
		case 0x3D: if(!FG&0x2000) NP = F; break;								// JNP
		case 0x3E: if(!CX) NP = F; break;										// JCXZ
		case 0x3F: runInt(F); break;
		default: error(cmd);
	}
}

void CPU::twoArg(ctype cmd, ctype dat){
	NP += 2;
	dtype second;
	if(dat >> 4 == 0){
		NP += 2;
		second = getSecond(dat,4);
	}
	else{
		second = getSecond(dat,2);
	}

	dtype first = getFirst(dat);

	// cout << hex << (dtype)cmd << "\t" << (dtype)dat << endl;
	// cout << hex << first << "\t" << second << endl;
	unsigned long tmp = first*second;

	switch(cmd){
		case 0x50: setFirst(dat, second); break;								// MOV
		case 0x52: doCMP(first,second); break;									// CMD
		case 0x53: setFirst(dat, first+second); break;							// ADD
		case 0x54: setFirst(dat, first-second); break;							// SUB
		case 0x55: AX = tmp&0xFFFF; DX = (dtype)(tmp >> 8); break;	// IMUL and IDIV
		case 0x56: AX = first/second; DX = first%second; break;
		case 0x57: setFirst(dat, first&second); break;							// AND
		case 0x59: setFirst(dat, first|second); break;							// OR
		case 0x5A: setFirst(dat, first^second); break;							// XOR
		case 0x5B: setFirst(dat, first<<second); break;							// SHL
		case 0x5C: setFirst(dat, first>>second); break;							// SHR
		default: error(cmd);
	}


}

void CPU::tick(){
	ctype cmd = (ctype)mem[IP];
	NP++;

	// cout << "CMD: " << bitset<8>(cmd) << endl;
	// cout << "CMD: 0x" << hex << (int)cmd << endl;

	if(cmd <= 0x0F){	// Check if command is in one-byte zone
		noArg(cmd);		// If it is, run the opcode
		IP = NP;
		return;			// Return
	}

	ctype dat = (ctype)mem[IP+1];	// If it isn't one byte, read another byte containing register and special information
	NP++;

	if(cmd >= 0x10 && cmd <= 0x2F){	// One argument?
		oneArgF(cmd,dat);
		IP = NP;
		return;
	}

	if(cmd >= 0x30 && cmd <= 0x4F){
		oneArgS(cmd,dat);
		IP = NP;
		return;
	}

	if(cmd >= 0x50 && cmd <= 0xFF){
		twoArg(cmd,dat);
		IP = NP;
		return;
	}

	// dump();
	cerr << "bad cmd\n";
	exit(1);
}

int main(){

	ctype* ram = new ctype[65536]{
		0x31, 0x04, 0x00, 0x1f,		// JMP to...
		0x00, 0x01,					// Multipliers
		0x00, 0x0A,
		0x00, 0x64,
		0x03, 0xE8,
		0x27, 0x10,
		0x6f, 0x76, 0x65, 0x72, 0x66, 0x6c, 0x6f, 0x77, 0x00,	// O V E R F L O W [null]
		0x6e, 0x61, 0x6e, 0x00,		// n a n [null] 
		0x00,						// padding
		0x00,
		0x13, 0xB0,					// POP (string addr) BX
		0x50, 0x93, 0x00, 0x00,

		// 0x50, 0xF4, 0x05, 0x00,			// MOV SP, 100
		// 0x50, 0xE1, 0xF0, 0x00,			// MOV BP, SP
		// 0x3F, 0x04, 0x00, 0x01,			// INT 0x01
		// 0x50, 0xB1, 0x90, 0x00,			// MOV CX, AX
		// 0x13, 0x90,						// POP AX
		// 0x04,							// DEC CX
		// 0x3F, 0x04, 0x00, 0x02,			// INT 0x02
		// 0x3E, 0x04, 0x00, 0x1F,			// JCXZ vv
		// 0x31, 0x04, 0x00, 0x10,			// JMP (pop)
		// 0x50, 0x94, 0x00, 0x0A,			// MOV AX, 0x05
		// 0x3F, 0x04, 0x00, 0x02,			// INT 0x02

		
		0x01 							// HLT
	};

	//cout << bitset<16>(hello >> 8) << endl;
	CPU FailDOS(ram);
	int steps = 0;

	while(!FailDOS.HLT && steps < 100000){
		FailDOS.tick();
		steps++;
		// FailDOS.dump();
	}
	FailDOS.dump();

	// for(int i = 0; i < 1000; i++){
	// 	cout << hex << (int)ram[i] << " ";
	// }
	cout << endl;


	delete[] ram;
	return 0;
}