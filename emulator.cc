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
int pc;

map<string, string> function_map;
//for function map, arguments read from left to right as arg1, arg2, and arg3

int mode;
//mode 0 = run to completion
//mode 1 = step through program


function_map.put("add",
		"arg1 = arg2 + arg3"
);

function_map.put ("addi",
		"arg1 = arg2 + arg3"
);

function_map.put("and",
		arg1 = arg2&arg3;
);

function_map.put("or",
		arg1 = arg2|arg3;
);

void parseLine(string lineOfRegistryCode) {

	//parse the line into the argument argument_name and arguments into arg1, arg2, and arg3
	switch (opcode) {
		case c1:
	}
}

void loadHelper(string argumentName, string arg1, string arg2, string arg3) {

	st

}

void add (int dreg, int a, int b){
	registers[dreg] = registers[a] + registers[b];
}

void addi (int dreg, int a, int c){
	registers[dreg] = registers[a] + c;
}

void addu (int dreg, int a, int b){

	int unsA = registers[a];
	int unsB = registers[b];
		if (unsA + u > 2^31-1)
			registers[dreg] = 2^31;
		else
			registers[dreg] = unsA + unsB;
}

void addiu (int dreg, int a, int c){
	int unsA = registers[a];
	int unsC = c;
		if (unsA + unsC > 2^31-1)
			registers[dreg] = 2^31;
		else
			registers[dreg] = unsA + unsC;
}

void andfunc (int dreg, int a, int b){
	registers[dreg] = registers[a] & registers[b];
}

void mult (int dreg, int a, int b){
	registers[dreg] =
}

void xorfunc (int dreg, int reg1, int reg2){
	registers[dreg] = (registers[reg1]&(!registers[reg2])) |
	(registers[reg2]&(!registers[reg1]));
}

void sll (int dreg, int a, int c){
	registers[dreg] = registers[a] << c;
}

void sra (int dreg, int a, int c){
	int i;
	int sum = 0;
	for(i = 1; i < c; i++){
		sum += 2^(31-i);
	}
	registers[dreg] = (registers[a] >> c) + sum;
}

void sub (int dreg, int a, int b){
	if(registers[a] - registers[b] < -2^31)
		registers[dreg] = -2^31;
	else if (registers[a] - registers[b] > 2^31-1)
		registers[dreg] = 2^31;
	else
		registers[dreg] = registers[a] - registers[b];
}

void subu (int dreg, int a, int b){
	registers[dreg] = registers[a] - registers[b];
}

void slt (int dreg, int a, int b){
	registers[dreg] = registers[a] < registers[b];
}

void slti (int dreg, int a, int c){
	registers[dreg] = registers[a] < c;
}

void sltu (int dreg, int a, int b){
	int a = registers[a];
	int b = registers[b];
	if(registers[a] < 0)
		a = registers[a]*(-1) +1;
	if(registers[b] < 0)
		b = registers[b]*(-1) +1
	registers[dreg] = registers[a] < registers[b];
}

void beq (int a, int b, int c){
	if(registers[a] == registers[b])
		pc += 4 + 4*c;
}

void bgez (int a, int c){
	if(registers[a] >= 0)
		pc += 4 + 4*c;
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
