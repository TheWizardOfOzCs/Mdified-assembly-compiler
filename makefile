EXEC_FILE = assembler
C_FILES = parser.c fileHandler.c lineHandler.c macrosHandler.c main.c utils.c secondpass.c processFile.c printToFiles.c labelHandler.c firstpass.c binaryCodeHandler.c

H_FILES = parser.c fileHandler.c lineHandler.h macrosHandler.h utils.h secondpass.c processFile.h printToFiles.h labelHandler.h firstpass.h binaryCodeHandler.h

CC:= gcc
CFLAGS:= -Wall -ansi -pedantic

O_FILES = $(C_FILES:.c=.o)

all: $(EXEC_FILE)

$(EXEC_FILE): $(O_FILES) 
	$(CC) $(CFLAGS) -g $(O_FILES) -o $(EXEC_FILE) -lm

%.o: %.c $(H_FILES)
	$(CC) $(CFLAGS) -g -c $(D_FLUSH) -o $@ $<

clean:
	rm -f *.o $(EXEC_FILE) *am
