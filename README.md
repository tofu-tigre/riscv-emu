# riscv-emu

## Description
This project is a 5-stage pipelined RISC-V CPU emulator created in the C++ language. It uses the traditional Fetch (IF), Decode (ID), Execute (EX), Memory (MEM), and Writeback (WB) pipeline as taught in *CS 152: Computer Architecture* at UC Berkeley.

## How to Use
Simply run the main.cpp file in your chosen IDE and watch your program execute. If you would like to see the instructions move through the CPU, simply define the keyword *DEBUG*. Further instructions are in the main.cpp file.

## Further Plans
* Implement IMEM and DMEM caches.
* Implement virtual memory translation along with translation lookaside buffer.
* Implement pre-fetcher.
* Implement branch predictor.
* Refactor code.
* Add more documentation.
## Credits
All code written by Jaeden Quintana. 