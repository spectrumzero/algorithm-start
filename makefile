CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = $(notdir $(wildcard *.c))
EXE = example

.PHONY: all clean

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE)
