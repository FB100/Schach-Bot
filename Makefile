# Compiler flags
# -lm to use math library
CFLAGS=-std=gnu11 -O2 -Wall -Wextra -Wpedantic -g
CFLAGSEND=-lm

.PHONY: all clean
all: main
# Added all files to be scanned for changes
# Hardcoded main.c as input files since other files are included in header
main: main.c init.c ai_evaluation.c ai_generation.c util.c tests.c
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGSEND)
clean:
	rm -f main
