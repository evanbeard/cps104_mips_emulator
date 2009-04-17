#include emulator.h
#include <vector>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int text[2*1024];
int staticData[4*1024];
int stack[2*1024];
//Be sure to consider that from the program's perspective, the text segment begins at address 0x00400000 and the static data segment begins at address 0x10010000
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

void getAddress(int address){
  if(address>0x7fffeffc && address < 0x00400000){
    return stack[address - 0x7fffeffc];
  }

  if(address>0x00400000 && address < 0x10010000){
    return text[address - 0x00400000];
  }

  if(address > 0x10010000){
    return staticData[address - 0x10010000];
	    }
}

void lb(int a, int b, int c){
  unsigned int bval = registers[b];
 registers[a] = getAddress(bval+registers[c]);
}

void lbu(int a, int b, int c){

 registers[a] = getAddress(registers[b]+registers[c]);
}

void lw(int a, int b, int c){

  int a = getAddress[registers[b]+registers[c]];
  //need to load address at registers[b]+register[c] and the three bytes after that address
  //how do I load an entire word aka put these 4 bytes together?

registers[a]=

}

void add(int a, int b, int c){

  registers[a] = registers[b] + registers[c];

}




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
