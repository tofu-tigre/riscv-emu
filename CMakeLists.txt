cmake_minimum_required(VERSION 3.24)
project(riscv_emu)

set(CMAKE_CXX_STANDARD 17)

add_executable(riscv_emu main.cpp CPU.cpp CPU.h memory/DRAM.cpp memory/DRAM.h consts.h Controller.cpp Controller.h ImmGenerator.cpp ImmGenerator.h ALU.cpp ALU.h BranchComp.cpp BranchComp.h RegisterArray.cpp RegisterArray.h Tickable.h FlipFlop.cpp FlipFlop.h memory/Cache.h memory/DirectMapCache.cpp memory/DirectMapCache.h)
