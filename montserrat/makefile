CC = gcc
P = soa

CDIR = src
ODIR = obj
IDIR = include

LIBS = -lm
CFLAGS = -Wall -Wextra -I $(IDIR)

SRC = $(wildcard $(CDIR)/*.c)
OBJ = $(patsubst $(CDIR)/%.c, $(ODIR)/%.o, $(SRC))
DEPS = $(wildcard $(IDIR)/*.h)

$(ODIR)/%.o: $(CDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(LIBS)

$(P): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

all: $(P)

.PHONY: clean

clean:
	rm -rf $(OBJ) $(P)
