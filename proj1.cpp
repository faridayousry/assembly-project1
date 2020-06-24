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
	funct7 = (instWord >> 25) & 0x0000007F;
	I_imm = (instWord >> 20) & 0x00000FFF;
	S_imm = ((instWord >> 7) & 0x0000001F) + ((instWord >> 25) & 0x0000007F);
	//B_imm = ((instWord >>));
	U_imm = (instWord >> 12) & 0x000FFFFF;
	//J_imm = ;

	
	if(opcode == 0x33){		// R Instructions (0110011) 51
		switch(funct3){
			case 0: if(funct7 == 32) {	
					cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] - regs[rs2];
				} else {
					cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] + regs[rs2];
				}
				  break;
			case 1: cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";		//SHOULD CASES BE IN HEXA????!!!!!!!!!!!
								//regs[rd] = regs[rs1] << regs[rs2];
				break;
			case 2: cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = (regs[rs1] < regs[rs2])?1:0
				break;

			case 3: cout << "\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = (regs[rs1] < regs[rs2])?1:0
				break;

			case 4: cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] ^ regs[rs2];
				break;

			case 5: if (I_imm == 32) {
					cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] >> regs[rs2];
			}
				  else {
					cout << "\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] >> regs[rs2];
			}
				  break;

			case 6: cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] | regs[rs2];
				break;

			case 7: cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
								//regs[rd] = regs[rs1] & regs[rs2];
				break;

			default: cout << "\tUnkown R Instruction \n";
		}
	} 
	else if (opcode == 0x13) {		// I-Instructions (0010011) 19 (andi, ori, slli...)
		switch (funct3) {
		case 0: cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = regs[rs1] - imm;
			break;

		case 1: cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = regs[rs1] << imm[0:4];
			break;

		case 2: cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = (regs[rs1] < imm)?1:0
			break;

		case 3: cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = (regs[rs1] < imm)?1:0
			break;

		case 4: cout << "\tXORI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = regs[rs1] ^ imm;
			break;

		case 5: if (funct7 == 32) {											//or  if(imm[5:11] == 23) ?????????????????????????
			cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = regs[rs1] >> imm[0:4];
		}
			  else {
			cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = regs[rs1] >> imm[0:4];
		}
			  break;

		case 6: cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = regs[rs1] | imm;
			break;

		case 7: cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
					//regs[rd] = regs[rs1] & imm;
			break;

		default: cout << "\tUnkown I Instruction \n";

		}
	}
	else if (opcode == 0x03) {		// I-Instructions (0000011) 3  (lb, lh, lw...)
		switch (funct3) {
		case 0: cout << "\tLB\tx" << rd << ", " << I_imm << "(" << rs1 << ")\n";	//ex: lb 0x3, 5(0x6)
					//regs[rd] = M[rs1 + imm][0:7]
			break;

		case 1: cout << "\tLH\tx" << rd << ", " << I_imm << "(" << rs1 << ")\n";	
					//regs[rd] = M[rs1 + imm][0:15]
			break;

		case 2: cout << "\tLW\tx" << rd << ", " << I_imm << "(" << rs1 << ")\n";
					//regs[rd] = M[rs1 + imm][0:31]
			break;

		case 4:  cout << "\tLBU\tx" << rd << ", " << I_imm << "(" << rs1 << ")\n";
					//regs[rd] = M[rs1 + imm][0:7]
			break;

		case 5: cout << "\tLHU\tx" << rd << ", " << I_imm << "(" << rs1 << ")\n";
					//regs[rd] = M[rs1 + imm][0:15]
			break;

		default: cout << "\tUnkown I Instruction \n";

		}
	}
	else if (opcode == 0x73) {          // ecall and ebreak (1110011) 115
		switch (I_imm) {
			case 0: cout << "\tECALL\t" << "\n";
				break;

			case 1: cout << "\tEBREAK\t" << "\n";
				break;

			default: cout << "\tUnkown R Instruction \n";
			}
		}
		else if (opcode == 0x67)         // Jalr (1100111) 103
			cout << "\tJALR\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
				//regs[rd] = PC + 4; PC += imm;

		else if (opcode == 0x23) {          // S-Instructions (0100011) 35
			switch (funct3) {
				case 0: cout << "\tSB\tx" << rs2 << ", " << S_imm << "(" << rs2 << ")" << "\n";
						//M[rs1 + imm][0:7] = rs2[0:7];
					break;

				case 1: cout << "\tSH\tx" << rs2 << ", " << S_imm << "(" << rs2 << ")" << "\n";
						//M[rs1 + imm][0:15] = rs2[0:15];
					break;

				case 2: cout << "\tSW\tx" << rs2 << ", " << S_imm << "(" << rs2 << ")" << "\n";
						//M[rs1 + imm][0:31] = rs2[0:31];
					break;
				}
			}
		else if (opcode == 0x63) {          // B-Instructions (1100011) 99
			switch (funct3) {}
		}

		else if (opcode == 0x6F) {          // J-Instructions (1101111) 111
			switch (funct3) {}
		}

		else if (opcode == 0x37)           // U-Instruction: LUI (0110111) 55
			cout << "\tLUI\tx" << rd << ", " << U_imm << "\n";
					//regs[rd] = imm << 12;
		
		else if (opcode == 0x17) {       // U-Instruction: AUIPC (0010111) 23
			cout << "\tAUPIC\tx" << rd << ", " << U_imm << "\n";
					//regs[rd] = PC + (imm << 12);

		}else 
			cout << "\tUnkown Instruction \n";
		
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
		
		while (true) {
			instWord = (unsigned char)buffer[pc] |
				(((unsigned char)buffer[pc + 1]) << 8) |
				(((unsigned char)buffer[pc + 2]) << 16) |
				(((unsigned char)buffer[pc + 3]) << 24);
			pc += 4;
			// remove the following line once you have a complete disassembler
			if (pc == 32) break;			// stop when PC reached address 32
			instDecExec(instWord);
		}
		

	} else emitError("Cannot access input file\n");
	//inFile.close();	??????/
	return 0;
}
