/*
	This is just a skeleton. It DOES NOT implement all the requirements.
	It only recognizes the "ADD" and "SUB" instructions and prints
	"Unkown Instruction" for all other instructions!

	References:
	(1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
	(2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>

using namespace std;

unsigned char buffer[8*1024];	// Buffer to hold the machine code

void emitError(char *s)
{
	cout << s;
	exit(0);
}


void instDecExec(unsigned int instWord)
{
	unsigned int rd, rs1, rs2, funct3, funct7, opcode;
	unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;

	opcode = instWord & 0x0000007F;			//to get rightmost 7 bits (OP Code)
	rd = (instWord >> 7) & 0x0000001F;		//to delete op code and get next 5 bits for rd (dest register)
	funct3 = (instWord >> 12) & 0x00000007;		//to delete 12 right most bits and get 3 bits for funct3
	rs1 = (instWord >> 15) & 0x0000001F;	//delete rightmost 15 bits and get 5 bits for rs1
	rs2 = (instWord >> 20) & 0x0000001F;	//delete rightmost 20 bits and get 5 bits for rs2

	if(opcode == 0x33){		// R Instructions (0110011) 51
		switch(funct3){
			case 0: if(funct7 == 32) {	
					cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] - regs[rs2];
				} else {
					cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] + regs[rs2];
				}
			case 1: cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";		//SHOULD CASES BE IN HEXA????!!!!!!!!!!!
								//regs[rd] = regs[rs1] << regs[rs2];

			case 2: cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = (regs[rs1] < regs[rs2])?1:0

			case 3: cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = (regs[rs1] < regs[rs2])?1:0

			case 4: cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] ^ regs[rs2];

			case 5: if (funct7 == 32) {
					cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] >> regs[rs2];
			}
				  else {
					cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] << regs[rs2];
			}

			case 6: cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] | regs[rs2];

			case 7: cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] & regs[rs2];

			break;																		//!!!!!!!!!!!!???????????here of after else in case 0?
			default: cout << "\tUnkown R Instruction \n";
		}
	} else {
		cout << "\tUnkown Instruction \n";
	}

}

int main(int argc, char *argv[]){

	unsigned int instWord=0;
	ifstream inFile;
	ofstream outFile;
	unsigned int pc = 0;

	if(argc<1) emitError("use: disasm <machine_code_file_name>\n");

	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if(inFile.is_open())
	{
		int fsize = inFile.tellg();

		inFile.seekg (0, inFile.beg);
		if(!inFile.read((char *)buffer, fsize)) emitError("Cannot read from input file\n");

		while(true){
				instWord = 	(unsigned char)buffer[pc] |
						(((unsigned char)buffer[pc+1])<<8) |
						(((unsigned char)buffer[pc+2])<<16) |
						(((unsigned char)buffer[pc+3])<<24);
				pc += 4;
				// remove the following line once you have a complete disassembler
				if(pc==32) break;			// stop when PC reached address 32
				instDecExec(instWord);
		}


	} else emitError("Cannot access input file\n");
}
