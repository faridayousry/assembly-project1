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

unsigned char buffer[8*1024];    // Buffer to hold the machine code

void emitError(char *s)
{
    cout << s;
    exit(0);
}




void instDecExec(unsigned int instWord)
{
    unsigned int rd, rs1, rs2, funct3, funct7, opcode, csr;
    unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
    unsigned int address;
    
    opcode = instWord & 0x0000007F;            //to get rightmost 7 bits (OP Code)
    rd = (instWord >> 7) & 0x0000001F;        //to delete op code and get next 5 bits for rd (dest register)
    funct3 = (instWord >> 12) & 0x00000007;        //to delete 12 right most bits and get 3 bits for funct3
    rs1 = (instWord >> 15) & 0x0000001F;    //delete rightmost 15 bits and get 5 bits for rs1
    rs2 = (instWord >> 20) & 0x0000001F;    //delete rightmost 20 bits and get 5 bits for rs2
    funct7 = (instWord >> 25) & 0x0000007F;
    csr = (instWord >> 20) & 0x0000000C;    //to get 12 leftmost bits for csr register (counter type instructions)
    
    I_imm = (instWord >> 20) & 0x00000FFF;
    S_imm = (((instWord >> 7) & 0x0000001F) + ((instWord >> 25) & 0x0000007F)) << 7;
    B_imm = (((instWord >> 8) & 0x0000000F)+ ((instWord >> 25) & 0x0000003F) + ((instWord >> 7) & 0x00000001) + ((instWord >> 30) & 0x00000001)) << 1;
    U_imm = ((instWord >> 12) & 0x000FFFFF) << 1;
    J_imm = (((instWord >> 21) & 0x000003FF) + ((instWord >> 20) & 0x00000001) + ((instWord >> 12) & 0x000000FF) + ((instWord >> 31) & 0x00000001)) << 1;
    
    unsigned int posNegBit_I = (instWord >> 31) & 0x00000001;
    unsigned int posNegBit_S = (instWord >> 31) & 0x00000001;
    unsigned int posNegBit_B = (instWord >> 31) & 0x00000001;
    unsigned int posNegBit_U = (instWord >> 31) & 0x00000001;
    unsigned int posNegBit_J = (instWord >> 31) & 0x00000001;
    
    
    if(opcode == 0x33){        // R Instructions (0110011) 51
        switch(funct3){
            case 0: if(funct7 == 32) {
                cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                //regs[rd] = regs[rs1] - regs[rs2];
            } else {
                cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                //regs[rd] = regs[rs1] + regs[rs2];
            }
                break;
            case 1: cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";        //SHOULD CASES BE IN HEXA????!!!!!!!!!!!
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
    
    
    else if (opcode == 0x13) {                      // I-Instructions (0010011) 19 (andi, ori, slli...)
        unsigned I_imm1, I_imm2;
        I_imm1 = (I_imm >> 5) & 0x0000007F;
        I_imm2 = I_imm & 0x0000001F;
        if(posNegBit_I == 0){
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
                
                    case 6: cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
                        //regs[rd] = regs[rs1] | imm;
                        break;
                
                    case 7: cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
                        //regs[rd] = regs[rs1] & imm;
                        break;
                
                    default: cout << "\tUnkown I Instruction \n";
                }
            }
        else{
            I_imm = (I_imm xor 0xFFF) + 1;
            switch (funct3) {
                case 0: cout << "\tANDI\tx" << rd << ", x" << rs1 << ", -" << I_imm << "\n";
                    //regs[rd] = regs[rs1] - imm;
                    break;
                    
                case 1: cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", -" << I_imm << "\n";
                    //regs[rd] = regs[rs1] << imm[0:4];
                    break;
                    
                case 2: cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", -" << I_imm << "\n";
                    //regs[rd] = (regs[rs1] < imm)?1:0
                    break;
                    
                case 3: cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", -" << I_imm << "\n";
                    //regs[rd] = (regs[rs1] < imm)?1:0
                    break;
                    
                case 4: cout << "\tXORI\tx" << rd << ", x" << rs1 << ", -" << I_imm << "\n";
                    //regs[rd] = regs[rs1] ^ imm;
                    break;
                    
                case 6: cout << "\tORI\tx" << rd << ", x" << rs1 << ", -" << I_imm << "\n";
                    //regs[rd] = regs[rs1] | imm;
                    break;
                    
                case 7: cout << "\tANDI\tx" << rd << ", x" << rs1 << ", -" << I_imm << "\n";
                    //regs[rd] = regs[rs1] & imm;
                    break;
                    
                default: cout << "\tUnkown I Instruction \n";
            }
        }
        if(funct3 = 5){
            if (I_imm1 == 32) {                                            //or  if(imm[5:11] == 23) ??????
                cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", " << I_imm2 << "\n";
                //regs[rd] = regs[rs1] >> imm[0:4];
            }
            else {
                cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", " << I_imm2 << "\n";
                //regs[rd] = regs[rs1] >> imm[0:4];
            }
        }
    }
    
    
    else if (opcode == 0x03) {        // I-Instructions (0000011) 3  (lb, lh, lw...)
        if(posNegBit_I == 0){
        switch (funct3) {
                case 0: cout << "\tLB\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";    //ex: lb 0x3, 5(0x6)
                    //regs[rd] = M[rs1 + imm][0:7]
                    break;
                
                case 1: cout << "\tLH\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
                    //regs[rd] = M[rs1 + imm][0:15]
                    break;
                
                case 2: cout << "\tLW\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
                    //regs[rd] = M[rs1 + imm][0:31]
                    break;
                
                case 4:  cout << "\tLBU\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
                    //regs[rd] = M[rs1 + imm][0:7]
                    break;
                
                case 5: cout << "\tLHU\tx" << rd << ", " << I_imm << "(x" << rs1 << ")\n";
                    //regs[rd] = M[rs1 + imm][0:15]
                    break;
                
                default: cout << "\tUnkown I Instruction \n";
            }
        }
         else {
            I_imm = (I_imm xor 0xFFF) + 1;
            switch (funct3) {
                case 0: cout << "\tLB\tx" << rd << ", -" << I_imm << "(x" << rs1 << ")\n";    //ex: lb 0x3, 5(0x6)
                    //regs[rd] = M[rs1 + imm][0:7]
                    break;
                    
                case 1: cout << "\tLH\tx" << rd << ", -" << I_imm << "(x" << rs1 << ")\n";
                    //regs[rd] = M[rs1 + imm][0:15]
                    break;
                    
                case 2: cout << "\tLW\tx" << rd << ", -" << I_imm << "(x" << rs1 << ")\n";
                    //regs[rd] = M[rs1 + imm][0:31]
                    break;
                    
                case 4:  cout << "\tLBU\tx" << rd << ", -" << I_imm << "(x" << rs1 << ")\n";
                    //regs[rd] = M[rs1 + imm][0:7]
                    break;
                    
                case 5: cout << "\tLHU\tx" << rd << ", -" << I_imm << "(x" << rs1 << ")\n";
                    //regs[rd] = M[rs1 + imm][0:15]
                    break;
                    
                default: cout << "\tUnkown I Instruction \n";
            }
        }
    }
    else if (opcode == 0x73) {          // ecall and ebreak (1110011) 115
        switch (I_imm) {
            case 0: cout << "\tECALL\t" << "\n";
                break;
                
            case 1: cout << "\tEBREAK\t" << "\n";
                break;

            default: cout << "\tUnkown I Instruction \n";
        }
        switch(funct3) {             //csrrw,csrrs...     
            case 1: cout << "\tCSRRW\t" << rd << ", "<< csr << ", "<< rs1<< "\n";
                break;
                //csrrw t0, csr,t1    

            case 2: cout << "\tCSRRS\t" << rd << ", "<< csr << ", "<< rs1<< "\n";
                break;
                //csrrs t0, csr, t1
                
            case 3: cout << "\tCSRRC\t" << rd << ", "<< csr << ", "<< rs1<< "\n";
                break;
                //csrrc t0 fcsr, t1

/*csrrwi t0 fcsr, 10
csrrsi t0 fcsr, 10
csrrci t0 fcsr, 10 */
            default: cout << "\tUnkown I Instruction \n";
        }
    }

    else if (opcode == 0x67){           // Jalr (1100111) 103
        cout << "\tJALR\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
    //regs[rd] = PC + 4; PC += imm;
    }
    else if (opcode == 0x23) {          // S-Instructions (0100011) 35
        if(posNegBit_S == 0){
           switch (funct3) {
               case 0: cout << "\tSB\tx" << rs2 << ", " << S_imm << "(x" << rs2 << ")" << "\n";
                   //M[rs1 + imm][0:7] = rs2[0:7];
                   break;
                
               case 1: cout << "\tSH\tx" << rs2 << ", " << S_imm << "(x" << rs2 << ")" << "\n";
                   //M[rs1 + imm][0:15] = rs2[0:15];
                   break;
                
               case 2: cout << "\tSW\tx" << rs2 << ", " << S_imm << "(x" << rs2 << ")" << "\n";
                   //M[rs1 + imm][0:31] = rs2[0:31];
                   break;

                default: cout << "\tUnkown S Instruction \n"; 

           }
        }
        else{
            S_imm = (S_imm xor 0xFFF) + 1;            
            switch (funct3) {
                case 0: cout << "\tSB\tx" << rs2 << ", -" << S_imm << "(x" << rs2 << ")" << "\n";
                    //M[rs1 + imm][0:7] = rs2[0:7];
                    break;
                    
                case 1: cout << "\tSH\tx" << rs2 << ", -" << S_imm << "(x" << rs2 << ")" << "\n";
                    //M[rs1 + imm][0:15] = rs2[0:15];
                    break;
                    
                case 2: cout << "\tSW\tx" << rs2 << ", -" << S_imm << "(x" << rs2 << ")" << "\n";
                    //M[rs1 + imm][0:31] = rs2[0:31];
                    break;

                default: cout << "\tUnkown S Instruction \n"; 

            }
        }
    }
    else if (opcode == 0x63) {          // B-Instructions (1100011) 99
        if(posNegBit_S == 0){
        switch (funct3) {
            case 0: cout << "\tBEQ\tx" << rs1 << ", " << rs2 << ", " << B_imm << "\n";
						//(rs1 == rs2)? PC += imm;
					break;

				case 1: cout << "\tBNE\tx" << rs1 << ", " << rs2 << ", " << B_imm << "\n";
						//(rs1 != rs2)? PC += imm;
					break;

				case 4: cout << "\tBLT\tx" << rs1 << ", " << rs2 << ", " << B_imm << "\n";
						//(rs1 < rs2)? PC += imm;
					break;

				case 5: cout << "\tBGE\tx" << rs1 << ", " << rs2 << ", " << B_imm << "\n";
						//(rs1 > rs2)? PC += imm;
					break;

				case 6: cout << "\tBLTU\tx" << rs1 << ", " << rs2 << ", " << B_imm << "\n";
						//(rs1 < rs2)? PC += imm;
					break;

				case 7: cout << "\tBGEU\tx" << rs1 << ", " << rs2 << ", " << B_imm << "\n";
						//(rs1 >= rs2)? PC += imm;
					break;


				default: cout << "\tUnkown B Instruction \n"; 
            }
        }
        else{
        B_imm = (B_imm xor 0xFFF) + 1;            
        switch (funct3) {
            case 0: cout << "\tBEQ\tx" << rs1 << ", " << rs2 << ", -" << B_imm << "\n";
						//(rs1 == rs2)? PC += imm;
					break;

				case 1: cout << "\tBNE\tx" << rs1 << ", " << rs2 << ", -" << B_imm << "\n";
						//(rs1 != rs2)? PC += imm;
					break;

				case 4: cout << "\tBLT\tx" << rs1 << ", " << rs2 << ", -" << B_imm << "\n";
						//(rs1 < rs2)? PC += imm;
					break;

				case 5: cout << "\tBGE\tx" << rs1 << ", " << rs2 << ", -" << B_imm << "\n";
						//(rs1 > rs2)? PC += imm;
					break;

				case 6: cout << "\tBLTU\tx" << rs1 << ", " << rs2 << ", -" << B_imm << "\n";
						//(rs1 < rs2)? PC += imm;
					break;

				case 7: cout << "\tBGEU\tx" << rs1 << ", " << rs2 << ", -" << B_imm << "\n";
						//(rs1 >= rs2)? PC += imm;
					break;


				default: cout << "\tUnkown B Instruction \n"; 
            }
        }

    }
    
    else if (opcode == 0x6F) {          // J-Instructions (1101111) 111
        cout << "\tJAL\tx" << rd << ", " << J_imm << "\n";
    }
    
    else if (opcode == 0x37)           // U-Instruction: LUI (0110111) 55
        cout << "\tLUI\tx" << rd << ", " << U_imm << "\n";
    //regs[rd] = imm << 12;
    
    else if (opcode == 0x17) {       // U-Instruction: AUIPC (0010111) 23
        cout << "\tAUPIC\tx" << rd << ", " << U_imm << "\n";
        //regs[rd] = PC + (imm << 12);
        
    }else if (opcode == 0x0F) {          // I-Instructions (00001111) 15
        switch (funct3) {
            case 0: cout << "\tFENCE\tx" << "\n";
						//fence succ,pred
					break;

			case 1: cout << "\tFENCE.I\tx" << "\n";
						//fence.i
					break;
        
        	default: cout << "\tUnkown I Instruction \n"; 

            }

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
            if (pc == fsize) break;
            instDecExec(instWord);
        }
        
        
    } else emitError("Cannot access input file\n");
    return 0;
}