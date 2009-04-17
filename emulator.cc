#include emulator.h
#include <vector>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int text[2*1024];
int staticData[4*1024];
int stack[2*1024];

int stack_pointer = 0x7fffefff;
int registers[32];

int mode;
//mode 0 = run to completion
//mode 1 = step through program

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
	// I-type
	int imm = instruction & 0xFFFF;
	// J-type
	int address = instruction & 0x3FFFFFF;
	
	switch (opcode) {
		case 0x0:
			// R type
			int funct = instruction & 0x3F;
			switch (funct):
				case 0x20:
					add(rd, rs, rt);
				case 0x21:
					addu(rd, rs, rt);
				case 0x24:
					commandAnd(rd, rs, rt); // need to call it something else
				case 0x8:
					jr(rs);
				case 0x10:
					mfhi(rs);
				case 0x12:
					mflo(rs);
				case 0x18:
					mult(rs, rt);
				case 0x19:
					multu(rs, rt);
				case 0x25:
					commandOr(rd, rs, rt);
				case 0x0:
					sll(rd, rt, shift);
				case 0x15A:
					slt(rd, rs, rt);
				case 0x15B:
					sltu(rd, rs, rt);
					
			
		case 0x08: //addi
			addi(rs, rt, imm);
		case 0x09:
			addiu(rs, rt, imm);
		
			
		default:
			cout << "not a valid instruction" << endl;
	}
}

void loadHelper(string argumentName, string arg1, string arg2, string arg3) {

	st

}

int main(int argc, char* argv[]) {
	cout << "argc = " << argc << endl;
	if (argv[1]=0) { //if user passes run to completion mode


	} else { //single step through program


		while (1) {
			string input;
			cin >> input;

			//      p reg print a specific register (e.g., p 4, prints the contents in hex of register 4)
			//	p all print the contents of all registers, including the PC, HI, & LO in hex
			//	d addr print the contents of memory location addr in hex, assume addr is a word address in hex.
			//	s n execute the next n instructions and stop (should print each instruction executed), then wait for the user to input another command 


			if (input == 'p') { //change this to if command starts with p, not equals p


				for(int i=0; i<32; i++;
						) {

							cout << registers[i] << endl;
						}

					}

			if (input =='d') {

			}

			if (input == 's') {

			}

		}

	}
}