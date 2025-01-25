// Computer Enhance Course Work.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum MovType {
	MovInvalid, // an invalid value used the mov can not be determined or is invalid
	MovRegMemToFromReg, // the mov instruction is [100010 d w] [ mod reg r/m ] [(DISP-LOW)] [(DISP-HI)]
	MovImediateToRegMem, // [1100011 w] [ mod 0 0 0 r/m] [(DISP-LOW)] [(DISP-HI)] [data] [data if w=1]
	MovImediateToReg, // [1011 w reg] [data] [data if w=1]
	MovMemToAccumulator, // [1010000 w ] [addr-lo] [addr-hi]
	MovAccumulatorToMem, // [1010010 w ] [addr-lo] [addr-hi]
	MovRegMemToSegReg, // [10001110] [mod 0 SR r/m] [(DISP-LO)] [(DISP-HI)]
	MovSegRegToRegMem // [10001100] [mod 0 SR r/m] [(DISP-LO)] [(DISP-HI)]
};

int main(int argc, char* args[])
{
	// quick test zone
	signed char testChar = 0b10000011;
	cout << (int)testChar << "\n";
	cout << abs(testChar) << "\n";

    cout << "Starting to read file\n";
    ofstream fileOut; // output file
    ifstream fileIn; // input file
    streampos size; 
    char* memBlock;

    // args test
    //cout << args[0] << "\n";
    //cout << argc << "\n";
	if (argc >= 2) {
		
	}
	else {
		cout << "specify a file name argument for the 8086 16 bit binary instructions file";
		return 0;
	}

    
    //file2.open("Listing-37.asm", ios::in|ios::binary|ios::ate);
    fileIn.open(args[1], ios::in | ios::binary | ios::ate);
    if (fileIn.is_open()) {
        // the ios::ate flag opens the file and sets the playhead 
        size = fileIn.tellg(); // tells you where the get position is;
        memBlock = new char[size]; 
        cout << "Input file is " << size << " bytes long. \n";
		// size is even so we can walk the file by 2 bytes per step
		fileIn.seekg(ios::beg); // start at the begining
		fileIn.read(memBlock, size); // read it into our buffer
		fileIn.close();

		cout << "fileOut: " << string(args[1]) + string("Decompiled.asm");
		fileOut.open(string(args[1]) + string("Decompiled.asm"));
		if (fileOut.is_open()) {
			cout << "fileOut opened okay \n";
			fileOut << "; =========================================================================\n";
			fileOut << "; "<< args[1] << " Decompiled\n";
			fileOut << "; =========================================================================\n";
			fileOut << "\n";
			fileOut << "bits 16\n";
			fileOut << "\n";
		}
		else {
			cout << "error opening file for output";
			return 0;
		}
		int numInstructionBytes = (int)size;
		//for (int i = 0; i < int(size); i=i+2) {
		//for (int i = 0; i < int(size); i=i+2) {
		bool notFinished = true;

		int i = 0; // index into memBlock;
		int bytesInInstruction = 2; // instructions are 1 - 4 bytes
		// for now lets expect 2 bytes min per instruction
		while (i < numInstructionBytes) {
			bytesInInstruction = 2;
			char firstByte = memBlock[i];
			char secondByte = memBlock[i+1];
			char thirdByte = 0; // loaded later if needed
			char fourthByte = 0;
			// check if instruction 

			// const char movInstruction = 0b10001000; // the mov instruction is [100010 d w ] bits 
			// kinds of move instructions
			const char movRegMemToFromReg = 0b10001000; // the mov instruction is [100010 d w] [ mod reg r/m ] [(DISP-LOW)] [(DISP-HI)]
			const char movImediateToRegMem = 0b11000110; // [1100011 w] [ mod 0 0 0 r/m] [(DISP-LOW)] [(DISP-HI)] [data] [data if w=1]
			const char movImediateToReg = 0b10110000; // [1011 w reg] [data] [data if w=1]
			const char movMemToAccumulator = 0b10100000; // [1010000 w ] [addr-lo] [addr-hi]
			const char movAccumulatorToMem = 0b10100010; // [1010010 w ] [addr-lo] [addr-hi]
			const char movRegMemToSegReg = 0b10001110; // [10001110] [mod 0 SR r/m] [(DISP-LO)] [(DISP-HI)]
			const char movSegRegToRegMem = 0b10001100; // [10001100] [mod 0 SR r/m] [(DISP-LO)] [(DISP-HI)]
			
			bool D = firstByte & 0b00000010;
			bool W = firstByte & 0b00000001;
			const char registerMOD = 0b11000000; // register Mode means the operands are registers not memory 
			const char memoryMOD = 0b00000000; // memory Mode means the operands are effective address calculations
			const char memoryDisp8MOD = 0b01000000; // memory plus Displaycement 8 bits Mode 
			const char memoryDisp16MOD = 0b10000000; // memory plus Displaycement 16 bits Mode 
			char instructionMOD = secondByte & registerMOD;
			char instructionREG = secondByte & 0b00111000;
			char instructionRM = secondByte & 0b00000111;
			int addressValue = 0; // assigned later
			signed char disp8 = 0; // assigned later
			signed char disp16 = 0; // assigned later


			// load byte 3 and 4 if its being used. going to assume we are not going to get an invalid instruction byte stream where we attempt to load bytes that are not there
			if ((instructionMOD == 0b00000000) && (instructionRM == 0b00000110)){ // special case for Direct Address transfer no EA calculation
				thirdByte = memBlock[i+2];
				fourthByte = memBlock[i+3];
				bytesInInstruction = 4;
			}
			if (instructionMOD == memoryDisp8MOD) {
				thirdByte = memBlock[i+2];
				bytesInInstruction = 3;
			}
			if (instructionMOD == memoryDisp16MOD) {
				thirdByte = memBlock[i+2];
				fourthByte = memBlock[i+3];
				bytesInInstruction = 4;
			}

			MovType movType = MovInvalid;
			if ((firstByte & movRegMemToFromReg) == movRegMemToFromReg) {
				movType = MovRegMemToFromReg;
			}
			else if ((firstByte & movImediateToRegMem) == movImediateToRegMem) {
				movType = MovImediateToRegMem;
			}
			else if ((firstByte & movImediateToReg) == movImediateToReg) {
				movType = MovImediateToReg;
			}
			else if ((firstByte & movMemToAccumulator) == movMemToAccumulator) {
				movType = MovMemToAccumulator;
			}
			else if ((firstByte & movRegMemToSegReg) == movRegMemToSegReg) {
				movType = MovRegMemToSegReg;
			}
			else if ((firstByte & movSegRegToRegMem) == movSegRegToRegMem) {
				movType = MovSegRegToRegMem;
			}



			if (movType == MovRegMemToFromReg) {
				fileOut << "mov ";
				if (instructionMOD == registerMOD) {
					// magic values from 8086 manual page 4-20 table 4-9. REG (Register) Field Encoding
					if (D) { // if Direction is 1 then REG is destination (first operand), else REG is source (second operand)
						// Register stored in REG
						switch (instructionREG) {
							case 0b00000000:
								fileOut << (W ? "ax" : "al");
								break;
							case 0b00001000:
								fileOut << (W ? "cx" : "cl");
								break;
							case 0b00010000:
								fileOut << (W ? "dx" : "dl");
								break;
							case 0b00011000:
								fileOut << (W ? "bx" : "bl");
								break;
							case 0b00100000:
								fileOut << (W ? "sp" : "ah");
								break;
							case 0b00101000:
								fileOut << (W ? "bp" : "ch");
								break;
							case 0b00110000:
								fileOut << (W ? "si" : "dh");
								break;
							case 0b00111000:
								fileOut << (W ? "di" : "bh");
								break;
						}
						fileOut << ", ";
						// register stored in the R/M 
						switch (instructionRM) {
							case 0b00000000:
								fileOut << (W ? "ax" : "al");
								break;
							case 0b00000001:
								fileOut << (W ? "cx" : "cl");
								break;
							case 0b00000010:
								fileOut << (W ? "dx" : "dl");
								break;
							case 0b00000011:
								fileOut << (W ? "bx" : "bl");
								break;
							case 0b00000100:
								fileOut << (W ? "sp" : "ah");
								break;
							case 0b00000101:
								fileOut << (W ? "bp" : "ch");
								break;
							case 0b00000110:
								fileOut << (W ? "si" : "dh");
								break;
							case 0b00000111:
								fileOut << (W ? "di" : "bh");
								break;
						}
					}
					else {

						// register stored in the R/M 
						switch (instructionRM) {
							case 0b00000000:
								fileOut << (W ? "ax" : "al");
								break;
							case 0b00000001:
								fileOut << (W ? "cx" : "cl");
								break;
							case 0b00000010:
								fileOut << (W ? "dx" : "dl");
								break;
							case 0b00000011:
								fileOut << (W ? "bx" : "bl");
								break;
							case 0b00000100:
								fileOut << (W ? "sp" : "ah");
								break;
							case 0b00000101:
								fileOut << (W ? "bp" : "ch");
								break;
							case 0b00000110:
								fileOut << (W ? "si" : "dh");
								break;
							case 0b00000111:
								fileOut << (W ? "di" : "bh");
								break;
						}
						fileOut << ", ";
						// Register stored in REG
						switch (instructionREG) {
							case 0b00000000:
								fileOut << (W ? "ax" : "al");
								break;
							case 0b00001000:
								fileOut << (W ? "cx" : "cl");
								break;
							case 0b00010000:
								fileOut << (W ? "dx" : "dl");
								break;
							case 0b00011000:
								fileOut << (W ? "bx" : "bl");
								break;
							case 0b00100000:
								fileOut << (W ? "sp" : "ah");
								break;
							case 0b00101000:
								fileOut << (W ? "bp" : "ch");
								break;
							case 0b00110000:
								fileOut << (W ? "si" : "dh");
								break;
							case 0b00111000:
								fileOut << (W ? "di" : "bh");
								break;
						}
					} // end if(D) 

				} else {
					fileOut << "???, ???\n";
				} // end if its a move between two registers
				fileOut << "\n";

			} // end if its a move instruction
			else if (instructionMOD == memoryMOD) {
				if (D) { // REG is dest
					switch (instructionREG) {
						case 0b00000000:
							fileOut << (W ? "ax" : "al");
							break;
						case 0b00001000:
							fileOut << (W ? "cx" : "cl");
							break;
						case 0b00010000:
							fileOut << (W ? "dx" : "dl");
							break;
						case 0b00011000:
							fileOut << (W ? "bx" : "bl");
							break;
						case 0b00100000:
							fileOut << (W ? "sp" : "ah");
							break;
						case 0b00101000:
							fileOut << (W ? "bp" : "ch");
							break;
						case 0b00110000:
							fileOut << (W ? "si" : "dh");
							break;
						case 0b00111000:
							fileOut << (W ? "di" : "bh");
							break;
					}
					cout << ", ";
					switch (instructionRM) {
					case 0b00000000:
						fileOut << "[bx + si]";
						break;
					case 0b00000001:
						fileOut << "[bx + di]";
						break;
					case 0b00000010:
						fileOut << "[bp + si]";
						break;
					case 0b00000011:
						fileOut << "[bp + di]";
						break;
					case 0b00000100:
						fileOut << "[si]";
						break;
					case 0b00000101:
						fileOut << "[di]";
						break;
					case 0b00000110:
						addressValue = thirdByte; // i think this should work, heh
						addressValue = addressValue << 8;
						addressValue += fourthByte;
						fileOut << "[" << addressValue << "]";
						break;
					case 0b00000111:
						fileOut << "[bx]";
						break;
					}

				}
				else { // D=0 REG is source
					switch (instructionRM) {
					case 0b00000000:
						fileOut << "[bx + si]";
						break;
					case 0b00000001:
						fileOut << "[bx + di]";
						break;
					case 0b00000010:
						fileOut << "[bp + si]";
						break;
					case 0b00000011:
						fileOut << "[bp + di]";
						break;
					case 0b00000100:
						fileOut << "[si]";
						break;
					case 0b00000101:
						fileOut << "[di]";
						break;
					case 0b00000110:
						addressValue = thirdByte; // i think this should work, heh
						addressValue = addressValue << 8;
						addressValue += fourthByte;
						fileOut << "[" << addressValue << "]";
						break;
					case 0b00000111:
						fileOut << "[bx]";
						break;
					}
					cout << ", ";
					switch (instructionREG) {
					case 0b00000000:
						fileOut << (W ? "ax" : "al");
						break;
					case 0b00001000:
						fileOut << (W ? "cx" : "cl");
						break;
					case 0b00010000:
						fileOut << (W ? "dx" : "dl");
						break;
					case 0b00011000:
						fileOut << (W ? "bx" : "bl");
						break;
					case 0b00100000:
						fileOut << (W ? "sp" : "ah");
						break;
					case 0b00101000:
						fileOut << (W ? "bp" : "ch");
						break;
					case 0b00110000:
						fileOut << (W ? "si" : "dh");
						break;
					case 0b00111000:
						fileOut << (W ? "di" : "bh");
						break;
					}
				}
			}
			else if (instructionMOD == memoryDisp8MOD) {
				string operandREG = "";
				string operandRM = "";
				switch (instructionREG) {
				case 0b00000000:
					operandREG = (W ? "ax" : "al");
					break;
				case 0b00001000:
					operandREG = (W ? "cx" : "cl");
					break;
				case 0b00010000:
					operandREG = (W ? "dx" : "dl");
					break;
				case 0b00011000:
					operandREG = (W ? "bx" : "bl");
					break;
				case 0b00100000:
					operandREG = (W ? "sp" : "ah");
					break;
				case 0b00101000:
					operandREG = (W ? "bp" : "ch");
					break;
				case 0b00110000:
					operandREG = (W ? "si" : "dh");
					break;
				case 0b00111000:
					operandREG = (W ? "di" : "bh");
					break;
				}
				switch (instructionRM) {
				case 0b00000000:
					operandRM = "[bx + si";
					break;
				case 0b00000001:
					operandRM = "[bx + di";
					break;
				case 0b00000010:
					operandRM = "[bp + si";
					break;
				case 0b00000011:
					operandRM = "[bp + di";
					break;
				case 0b00000100:
					operandRM = "[si";
					break;
				case 0b00000101:
					operandRM = "[di";
					break;
				case 0b00000110:
					operandRM = "[bp";
					break;
				case 0b00000111:
					operandRM = "[bx";
					break;
				}
				// add displacement
				disp8 = thirdByte;
				if (disp8 < 0) {
					operandRM += " - " + to_string(abs(disp8));
				}
				else {
					operandRM += " + " + to_string(disp8);
				}
				operandRM += "]";


				if (D) { // arrange operands based on direction, is REG dest or source ?
					fileOut << operandREG << ", " << operandRM;
				}
				else {
					fileOut << operandRM << ", " << operandREG;
				}

			}
			else if (instructionMOD == memoryDisp16MOD) {
				int x = 0;

				string operandREG = "";
				string operandRM = "";
				switch (instructionREG) {
				case 0b00000000:
					operandREG = (W ? "ax" : "al");
					break;
				case 0b00001000:
					operandREG = (W ? "cx" : "cl");
					break;
				case 0b00010000:
					operandREG = (W ? "dx" : "dl");
					break;
				case 0b00011000:
					operandREG = (W ? "bx" : "bl");
					break;
				case 0b00100000:
					operandREG = (W ? "sp" : "ah");
					break;
				case 0b00101000:
					operandREG = (W ? "bp" : "ch");
					break;
				case 0b00110000:
					operandREG = (W ? "si" : "dh");
					break;
				case 0b00111000:
					operandREG = (W ? "di" : "bh");
					break;
				}
				switch (instructionRM) {
				case 0b00000000:
					operandRM = "[bx + si";
					break;
				case 0b00000001:
					operandRM = "[bx + di";
					break;
				case 0b00000010:
					operandRM = "[bp + si";
					break;
				case 0b00000011:
					operandRM = "[bp + di";
					break;
				case 0b00000100:
					operandRM = "[si";
					break;
				case 0b00000101:
					operandRM = "[di";
					break;
				case 0b00000110:
					operandRM = "[bp";
					break;
				case 0b00000111:
					operandRM = "[bx";
					break;
				}
				// add displacement
				disp16 = fourthByte;
				disp16 = disp16 << 8;
				disp16 = disp16 + thirdByte;
				if (disp16 < 0) {
					operandRM += " - " + to_string(abs(disp16));
				}
				else {
					operandRM += " + " + to_string(disp16);
				}
				operandRM += "]";


				if (D) { // arrange operands based on direction, is REG dest or source ?
					fileOut << operandREG << ", " << operandRM;
				}
				else {
					fileOut << operandRM << ", " << operandREG;
				}
			}

			
			i += bytesInInstruction;
		} // end while(notFinished)
		fileOut.close();

    }
    else {
        cout << "error opening file";
    }

    //fileOut.open("test.txt");
    //if (fileOut.is_open()) {
    //    fileOut << "Line 1 test\n";
    //    fileOut << "Line 2 test\n";
    //    fileOut.close();
    //    cout << "wrote 2 test lines";
    //    

    //}
    //else {
    //    cout << "unable to open file";
    //}

	cout << "\n";
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
