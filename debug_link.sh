#!/bin/bash
cd F:/0_TurnBaseAdventure
CRT_DIR="C:/msys64/ucrt64/lib"
GCC_LIB="C:/msys64/ucrt64/lib/gcc/x86_64-w64-mingw32/16.1.0"

# Run ld with the same args but add --trace-symbol to see unresolved symbols  
ld.exe \
  -m i386pep \
  -Bdynamic \
  -o test_minimal2.exe \
  $GCC_LIB/../../../../lib/crt2.o \
  $GCC_LIB/crtbegin.o \
  -L$GCC_LIB \
  -L$GCC_LIB/../../../../x86_64-w64-mingw32/lib/../lib \
  -L$GCC_LIB/../../../../lib \
  -L$GCC_LIB/../../../../x86_64-w64-mingw32/lib \
  -L$GCC_LIB/../../.. \
  main.o Character.o \
  -lstdc++ -lmingw32 -lgcc_s -lgcc -lmingwex -lmsvcrt -lkernel32 -lpthread -ladvapi32 -lshell32 -luser32 \
  $GCC_LIB/../../../../lib/default-manifest.o \
  $GCC_LIB/crtend.o \
  2>&1
