CC      = gcc
CFLAGS  = -Wall -g -std=c99 -pedantic -O

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assembler/assemble.o assembler/assembler.o assembler/binary_converter.o assembler/struct_converter.o utils/hash_table.o utils/split_string.o utils/arm11.o
	$(CC) $(CFLAGS) -o assemble assembler/assemble.o assembler/assembler.o assembler/binary_converter.o assembler/struct_converter.o utils/hash_table.o utils/split_string.o utils/arm11.o

emulate: emulator/emulate.o emulator/emulator.o emulator/fetch.o emulator/decode.o emulator/execute.o utils/arm11.o
	$(CC) $(CFLAGS) -o emulate emulator/emulate.o emulator/emulator.o emulator/fetch.o emulator/decode.o emulator/execute.o utils/arm11.o

assembler/assemble.o: assembler/assemble.c
	$(CC) $(CFLAGS) -o assembler/assemble.o -c assembler/assemble.c

assembler/assembler.o: assembler/assembler.c
	$(CC) $(CFLAGS) -o assembler/assembler.o -c assembler/assembler.c

assembler/binary_converter.o: assembler/binary_converter.c
	$(CC) $(CFLAGS) -o assembler/binary_converter.o -c assembler/binary_converter.c

assembler/struct_converter.o: assembler/struct_converter.c
	$(CC) $(CFLAGS) -o assembler/struct_converter.o -c assembler/struct_converter.c

emulator/emulate.o: emulator/emulate.c
	$(CC) $(CFLAGS) -o emulator/emulate.o -c emulator/emulate.c

emulator/fetch.o: emulator/fetch.c
	$(CC) $(CFLAGS) -o emulator/fetch.o -c emulator/fetch.c

emulator/decode.o: emulator/decode.c
	$(CC) $(CFLAGS) -o emulator/decode.o -c emulator/decode.c

emulator/execute.o: emulator/execute.c
	$(CC) $(CFLAGS) -o emulator/execute.o -c emulator/execute.c

emulator/emulator.o: emulator/emulator.c
	$(CC) $(CFLAGS) -o emulator/emulator.o -c emulator/emulator.c

utils/hash_table.o: utils/hash_table.c
	$(CC) $(CFLAGS) -o utils/hash_table.o -c utils/hash_table.c

utils/split_string.o: utils/split_string.c
	$(CC) $(CFLAGS) -o utils/split_string.o -c utils/split_string.c

utils/arm11.o: utils/arm11.c
	$(CC) $(CFLAGS) -o utils/arm11.o -c utils/arm11.c

clean:
	rm -f $(wildcard assembler/*.o)
	rm -f $(wildcard emulator/*.o)
	rm -f $(wildcard utils/*.o)
	rm -f assemble
	rm -f emulate
