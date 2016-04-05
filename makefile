CC=cl.exe
BFLAGS=/nologo /Ox
CFLAGS=/nologo /Ox /c

main.exe: main.cpp Int.cpp add.cpp sub.cpp mul.cpp div.cpp misc.cpp mini-gmp.obj
  $(CC) $(BFLAGS) main.cpp /link mini-gmp.obj

main.exe asm : main.cpp Int.cpp add.cpp sub.cpp mul.cpp div.cpp misc.cpp add.obj sub.obj mul.obj div.obj mini-gmp.obj
  $(CC) $(BFLAGS) main.cpp /link add.obj sub.obj mul.obj div.obj mini-gmp.obj

add.obj : add.asm
  nasm.exe -f win32 -Xvc -o add.obj add.asm

sub.obj : sub.asm
  nasm.exe -f win32 -Xvc -o sub.obj sub.asm

mul.obj : mul.asm
  nasm.exe -f win32 -Xvc -o mul.obj mul.asm

div.obj : div.asm
  nasm.exe -f win32 -Xvc -o div.obj div.asm

mini-gmp.obj : mini-gmp.c
  $(CC) $(CFLAGS) mini-gmp.c

clear : 
  del *.obj
  del main.exe