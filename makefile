CC = gcc
P = soa

CDIR = ./src
_SRC = %.c
SRC = $(patsubst %,$(CDIR)/%,$(_SRC))

ODIR = ./obj
_OBJ = fat32.o ext4.o identifier.o argval.o view.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

IDIR = ./include
_DEPS = types.h fat32.h ext4.h identifier.h argval.h view.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CFLAGS = -lm -Wall -Wextra -I $(IDIR)

$(ODIR)/%.o : $(SRC) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(P): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

all: $(P)
	
.PHONY: clean

clean:
	rm -f $(OBJ) $(P)