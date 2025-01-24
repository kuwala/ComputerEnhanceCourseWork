# Computer Enhance Course Work readme.md

## Log
commit `2c60315` is for listing 37 and 37.
```md
mov Instruction Decoder

`mov AX, BX` encoded as two bytes

[10010DW] [MOD REG R/M]
[6bits D W] [2 bits, 3 bits, 3 bits]
- D is direction of REG register
- W is byte or word

mod specifies if its a memory or a register operation
2nd byte 
Reg field encodes a register, 
[11, encodes register, also encodes register (when 11 detected in MOD field)]
if D bit is 0 REG field is not dest, if D is 1 register is the Destination.
Flipping the D field would change What the REG is and the R/W would be the other one.
- W stands for wide and encodes if the registers are 8bits or 16bits 

MOV AX, BX
MOV AL, BL 

- c++ has char instead of byte data type ... interesting 

## the 8086 manual
8086 manual : https://edge.edx.org/c4x/BITSPilani/EEE231/asset/8086_family_Users_Manual_1_.pdf

To test it you can run nasm on the output and the run the binary from nasm through
the code again and see if any of the data/text in the files change.

nasm : Netwide Assembler for x86 CPU architecture https://www.nasm.us/
```


