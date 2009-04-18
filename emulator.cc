//#include "emulator.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int text[2*1024 / 4];
int staticData[4*1024 / 4];
int stack[2*1024 / 4];
//Be sure to consider that from the program's perspective, the text segment begins at address 0x00400000 and the static data segment begins at address 0x10010000
int stack_pointer = 0x7fffefff;
int registers[32];
int pc;
int hireg;
int loreg;

int mode;
//mode 0 = run to completion
//mode 1 = step through program

int getAddress(int address) {
	if (address>0x7fffeffc && address < 0x00400000) {
		return stack[address - 0x7fffeffc];
	}

	if (address>0x00400000 && address < 0x10010000) {
		return text[address - 0x00400000];
	}

	if (address > 0x10010000) {
		return staticData[address - 0x10010000];
	}
}

int storeAddress(int address, int wordToStore) {
	if (address>0x7fffeffc && address < 0x00400000) {
		return stack[address - 0x7fffeffc] = wordToStore;
	}

	if (address>0x00400000 && address < 0x10010000) {
		return text[address - 0x00400000] = wordToStore;

	}
	if (address > 0x10010000) {
		return staticData[address - 0x10010000] = wordToStore;
	}
}

void lb(int a, int b, int c) {

	registers[a] = getAddress(b+registers[c]);
}

void lbu(int a, unsigned int b, int c) {

	registers[a] = getAddress(b+registers[c]);
}

void lw(int a, int b, int c) {

  registers[a] =
    (getAddress(b+registers[c]))
 + 
    (getAddress(b+registers[c]+1) << 8)
    + (getAddress(b+registers[c]+2) << 16) 
+ (getAddress(b+registers[c]+3) << 24);
}

void sb(int a, int b, int c) {
	storeAddress(b+registers[c], registers[a] & 0xFF); //0xFF = 8 one's in a row to get first byte
}

void sw(int a, int b, int c) {
	storeAddress(b+registers[c], registers[a] & 0xFF);
	storeAddress(b+registers[c] + 1, (registers[a] & 0xFF00) >> 8);
	storeAddress(b+registers[c] + 2, (registers[a] & 0xFF0000) >> 16);
	storeAddress(b+registers[c] + 3, (registers[a] & 0xFF000000) >> 24);
}

void lui(int a, int b) {
	registers[a] = b << 16;
}

void add(int dreg, int a, int b) {
	registers[dreg] = registers[a] + registers[b];
}

void addi(int dreg, int a, int c) {
	registers[dreg] = registers[a] + c;
}

void addu(int dreg, int a, int b) {

	unsigned int unsA = registers[a];
	unsigned int unsB = registers[b];
	registers[dreg] = unsA + unsB;
}

void addiu(int dreg, int a, int c) {
	unsigned int unsA = registers[a];
	unsigned int unsC = c;
	registers[dreg] = unsA + unsC;
}

void andfunc(int dreg, int a, int b) {
	registers[dreg] = registers[a] & registers[b];
}

void mult(int a, int b) {
	long product = registers[a] * registers[b];
	hireg = product >> 32;
	loreg = (product << 32) >> 32;
}

void multu(int a, int b) {
	unsigned int unsA = registers[a];
	unsigned int unsB = registers[b];
	long product = unsA * unsB;
	hireg = product >> 32;
	loreg = (product << 32) >> 32;
}

void orfunc(int dreg, int a, int b) {
	registers[dreg] = registers[a] | registers[b];
}

void ori(int dreg, int a, int c) {
	registers[dreg] = registers[a] | c;
}

void xorfunc(int dreg, int reg1, int reg2) {
	registers[dreg] = (registers[reg1]&(!registers[reg2])) | (registers[reg2]
			&(!registers[reg1]));
}

void sll(int dreg, int a, int c) {
	registers[dreg] = registers[a] << c;
}

void sra(int dreg, int a, int c) {
	int i;
	int sum = 0;
	for (i = 1; i < c; i++) {
		sum += 2^(31-i);
	}
	registers[dreg] = (registers[a] >> c) + sum;
}

void srl(int dreg, int a, int c) {
	registers[dreg] = registers[a] >> c;
}

void sub(int dreg, int a, int b) {
	registers[dreg] = registers[a] - registers[b];
}

void subu(int dreg, int a, int b) {
	unsigned int unsA = registers[a];
	unsigned int unsB = registers[b];
	registers[dreg] = unsA - unsB;
}

void slt(int dreg, int a, int b) {
	if(registers[a] < registers[b])
		registers[dreg] = 1;
	else
		registers[dreg] = 0;
}

void slti(int dreg, int a, int c) {
	if(registers[a] < c)
		registers[dreg] = 1;
	else
		registers[dreg] = 0;
}

void sltu(int dreg, int a, int b) {
	unsigned int a = registers[a];
	unsigned int b = registers[b];
	if(a < b)
		registers[dreg] = 1;
	else
		registers[dreg] = 0;
}

void sltiu(int dreg, int a, int c) {
	unsigned int a = registers[a];
	unsigned int c = c;
	if(a < c)
		registers[dreg] = 1;
	else
		registers[dreg] = 0;
}

void beq(int a, int b, int c) {
	if (registers[a] == registers[b])
		pc += 1 + 1*c;
}

void bgez(int a, int c) {
	if (registers[a] >= 0)
		pc += 1 + 1*c;
}

void bgtz(int a, int c) {
	if (registers[a] > 0)
		pc += 1 + 1*c;
}

void blez(int a, int c) {
	if (registers[a] <= 0)
		pc += 1 + 1*c;
}

void bltz(int a, int c) {
	if (registers[a] < 0)
		pc += 1 + 1*c;
}

void bne(int a, int b, int c) {
	if (registers[a] != registers[b])
		pc += 1 + 1*c;
}

void jump(int c) {
	pc = c;
}

void jal(int c) {
	pc = c;
	registers[31] = pc + 1;
}

void jr(int a) {
	pc = registers[a];
}

void mfhi(int a) {
	registers[a] = hireg;
}

void mflo(int a) {
	registers[a] = loreg;
}

void syscall() {
	int v0 = registers[2]; // register 2 is v0
	switch (v0) {
	case 1:
		printf("%i", registers[4]); //registers 4-7 are a0-a3
		break;
	case 4:
	  printf("%s", registers[4]); //registers 4-7 are a0-a3
		break;
	case 5:
		scanf("%i", &v0);
		break;
	case 8:
		char str [80];
//		int a0 = registers[4];
//		int a1 = registers[5]; 
		scanf("%s", str);
		registers[4] = (int) &str[0];
		registers[5] = sizeof(str)/sizeof(char) + 1;
		break;
	case 10:
		exit(1);
		break;
	}
}

/*
 LB load byte
 LBU load byte unsigned
 LW load word
 SB store byte
 SW store word
 LUI load upper immediate
 ADD add
 ADDI add immediate
 ADDU add without overflow
 ADDIU add immediate without overflow
 AND and
 MULT multiply
 MULTU unsigned multiply
 OR or
 ORI or immediate
 XOR xor
 SLL shift left logical
 SRA shift right arithmetic
 SRL shift right logical
 SUB subtract with overflow
 SUBU subtract without overflow
 SLT set less than
 SLTI set less than immediate
 SLTU set less than unsigned
 SLTIU set less than immediate unsigned
 BEQ branch on equal
 BGEZ branch on greater than equal zero
 BGTZ branch on greater than zero
 BLEZ branch on less than equal zero
 BLTZ branch on less than zero
 BNE branch on not equal
 J jump
 JAL jump and link
 JR jump register
 MFHI move from HI register
 MFLO move from LO register
 SYSCALL system call-like facilities that SPIM programs can use (implement syscall code 1,4,5,8,10)
 */

void parseLine(int instruction) {

	// increment program pointer
	pc += 4;

	//parse registry code
	int opcode = (instruction & 0xFC000000) >> 26;
	// R-type
	int rs = (instruction & 0x3E00000) >> 21;
	int rt = (instruction & 0x1F0000) >> 16;
	int rd = (instruction & 0xF800) >> 11;
	int shift = (instruction & 0x7C0) >> 6;
	int funct = instruction & 0x3F;

	// I-type
	int imm = instruction & 0xFFFF;
	// J-type
	int address = instruction & 0x3FFFFFF;

	switch (opcode) {
	case 0x0:
		// R type
		switch (funct) {
		case 0x20:
			add(rd, rs, rt);
			break;
		case 0x21:
			addu(rd, rs, rt);
			break;
		case 0x24:
			andfunc(rd, rs, rt); // need to call it something else
			break;
		case 0x8:
			jr(rs);
			break;
		case 0x10:
			mfhi(rs);
			break;
		case 0x12:
			mflo(rs);
			break;
		case 0x18:
			mult(rs, rt);
			break;
		case 0x19:
			multu(rs, rt);
			break;
		case 0x25:
			orfunc(rd, rs, rt);
			break;
		case 0x0:
			sll(rd, rt, shift);
			break;
		case 0x15A:
			slt(rd, rs, rt);
			break;
		case 0x15B:
			sltu(rd, rs, rt);
			break;
		case 0x3:
			sra(rs, rt, shift);
			break;
		case 0x2:
			srl(rs, rt, shift);
			break;
		case 0x22:
			sub(rd, rs, rt);
			break;
		case 0x23:
			subu(rd, rs, rt);
			break;
		case 0xC:
			syscall();
			break;
		case 0x26:
			xorfunc(rd, rs, rt);
			break;
		}
		break;
		// I-type
	case 0x08: //addi
		addi(rs, rt, imm);
		break;
	case 0x09:
		addiu(rs, rt, imm);
		break;
	case 0x4:
		beq(rs, rt, imm);
		break;
	case 0x1:
		switch (rt) {
		case 1:
			bgez(rs, imm);
			break;
		case 0:
			bltz(rs, imm);
			break;
		}
		break;
	case 0x7:
		if (rt == 0)
			bgtz(rs, imm);
		break;
	case 0x6:
		if (rt == 0)
			blez(rs, imm);
		break;
	case 0x5:
		bne(rs, rt, imm);
		break;
	case 0x20:
		lb(rt, imm, rs);
		break;
	case 0x24:
		lbu(rt, imm, rs);
		break;
	case 0xF:
		lui(rt, imm);
		break;
	case 0x23:
		lw(rt, imm, rs);
		break;
	case 0xD:
		ori(rt, rs, imm);
		break;
	case 0x28:
		sb(rt, imm, rs);
		break;
	case 0xA:
		slti(rt, rs, imm);
		break;
	case 0xB:
		sltiu(rt, rs, imm);
		break;
	case 0x2B:
		sw(rt, imm, rs);
		break;
		// J-type
	case 0x2:
		jump(address);
		break;
	case 0x3:
		jal(address);
		break;
	default:
		cout << "not a valid instruction" << endl;
		break;
	}
}

void readFile(string filename) {
	string line;
	ifstream myfile(filename.c_str());
	vector<string> entireFile;
	if (myfile.is_open()) {

		int i=0;
		while (!myfile.eof() ) {
			getline(myfile, line);
			entireFile[i]=line;
			i++;
		}
		myfile.close();
	}

	int j;
	for (j=0; j<entireFile.size(); j++) {

		if (entireFile[j] == "DATA SEGMENT\n") {
			break;
		}
		

		int* current =  &entireFile[j];
		text[j]=atoi(current);
	}


	int k;
	for (k = 0; k<entireFile.size() - j; k++) {
		string first =entireFile[1+j+k];
		string::size_type pos;
		pos=first.find(' ', 0);
		string second=first.substr(pos, 0);
		string first=first.substr(0, pos);
		int first = atoi(first);
		int second = atoi(second);
		storeAddress(first, second);
	}

}

int main(int argc, char* argv[]) {
	cout << "argc = " << argc << endl;

	if (argv[1] == 0) { //if user passes run to completion mode


	} else { //single step through program


		while (1) {
			string input;
			cin >> input;

			//  p reg print a specific register (e.g., p 4, prints the contents in hex of register 4)
			//	p all print the contents of all registers, including the PC, HI, & LO in hex
			//	d addr print the contents of memory location addr in hex, assume addr is a word address in hex.
			//	s n execute the next n instructions and stop (should print each instruction executed), then wait for the user to input another command


			if (input == "p all") {
				for (int i=0; i<32; i++) {
					printf("%x", &registers[i]);
				}
			} else if (input.at(0) == 'p') {
				int regnum = atoi(input.substr(2, input.size()-2));
				printf("%x", &registers[regnum]);
			}

			if (input =="d addr") {
				int addr = atoi(input.substr(2, input.size()-2));
				printf("%x", &registers[regnum]);
			}

			if (input == "s") {
				int num = atoi(input.substr(2, input.size()-2));
				int i;
				int instr;
				for (int i = 0; i < num; i++) {
					instr = text[pc];
					cout << "Instruction: " << instr << end1;
					parseLine(instr);
				}

			}

		}
	}
