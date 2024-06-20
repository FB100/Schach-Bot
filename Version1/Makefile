# Compiler flags
# -lm to use math library
CFLAGS=-std=gnu11 -O2 -Wall -Wextra -Wpedantic -g
CFLAGSDEBUG = -O0 -ggdb3 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic -g
CFLAGSEND=-lm

FILES = main.c ai_evaluation.c ai_generation.c util.c tests.c zobrist_hashing.c repetition_table.c board.c additional_functionality/precompute_bitboards.c

.PHONY: all clean
all: main
# Added all files to be scanned for changes
# Hardcoded main.c as input files since other files are included in header
main: $(FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGSEND)
debug: $(FILES)
	$(CC) $(CFLAGSDEBUG) -o $@ $^ $(CFLAGSEND)
clean:
	rm -f main
	rm -f debug
