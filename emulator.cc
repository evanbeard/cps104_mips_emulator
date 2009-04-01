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

map<string, string> function_map;
//for function map, arguments read from left to right as arg1, arg2, and arg3

int mode;
//mode 0 = run to completion
//mode 1 = step through program


function_map.put("add",

		arg1 = arg2 + arg3;
);

function_map.put ("addi",
		arg1 = arg2 + arg3
);

function_map.put("and",
		arg1 = arg2&arg3;
);

function_map.put("or",
		arg1 = arg2|arg3;
);

void parseLine(string lineOfRegistryCode) {

	//parse the line into the argument argument_name and arguments into arg1, arg2, and arg3

	loadHelper(argumentName, arg1, arg2, arg3);

}

void loadHelper(string argumentName, string arg1, string arg2, string arg3) {

	st

}

int main(int argc, char* argv[])
{
	cout << "argc = " << argc << endl;
	if (argv[1]=0) { //if user passes run to completion mode


	} else { //single step through program


		while(1) {
			string input;
			cin >> input;

			//      p reg print a specific register (e.g., p 4, prints the contents in hex of register 4)
			//	p all print the contents of all registers, including the PC, HI, & LO in hex
			//	d addr print the contents of memory location addr in hex, assume addr is a word address in hex.
			//	s n execute the next n instructions and stop (should print each instruction executed), then wait for the user to input another command 


			if (input == 'p') { //change this to if command starts with p, not equals p


				for(int i=0; i<32; i++;) {

					cout << registers[i] << endl;
				}

			}

			if (input =='d') {

			}

			if (input == 's') {

			}

		}

	}
