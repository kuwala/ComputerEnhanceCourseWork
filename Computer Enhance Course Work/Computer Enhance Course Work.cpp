// Computer Enhance Course Work.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* args[])
{
    cout << "Starting to read file\n";
    ofstream fileOut; // output file
    ifstream fileIn; // input file
    streampos size; 
    char* memblock;

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
        memblock = new char[size]; 
        cout << "Input file is " << size << " bytes long. \n";
		if (size % 2 == 0) {
			// size is even so we can walk the file by 2 bytes per step
			fileIn.seekg(ios::beg); // start at the begining
			fileIn.read(memblock, size); // read it into our buffer
			fileIn.close();

			fileOut.open(string(args[1]) + string("Decompiled.asm"));
			if (fileOut.is_open()) {
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
            int numInstructions = (int)size / 2;
            for (int i = 0; i < int(size); i=i+2) {
                char firstByte = memblock[i];
                char secondByte = memblock[i+1];

                const char movInstruction = 0b10001000; // the mov instruction is [100010 + D + W ] bits 
                bool D = firstByte & 0b00000010;
                bool W = firstByte & 0b00000001;
                const char registerMOD = 0b11000000; // register Mode means the operands are registers not memory 
                char instructionMOD = secondByte & registerMOD;
                char instructionREG = secondByte & 0b00111000;
                char instructionRM = secondByte & 0b00000111;



                if ((firstByte & movInstruction) == movInstruction) {
                    fileOut << "mov ";
                    if (instructionMOD == registerMOD) {
                        // magic values from 8086 manual page 4-20 table 4-9. REG (Register) Field Encoding
                        if (D) { // if Direction is 1 then REG is destination, else REG is source
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

            } // end of for loop
			fileOut.close();

        }
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
