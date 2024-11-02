CC = clang

SRCS = $(wildcard src/*.cpp)

OUT32 = injector32.exe
OUT64 = injector64.exe

all: m32 m64

m32:
	$(CC) -m32 -o .\bin\$(OUT32) $(SRCS)

m64:
	$(CC) $(FLAGS) -o .\bin\$(OUT64) $(SRCS)