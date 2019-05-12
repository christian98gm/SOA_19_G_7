CC = gcc
P = soa

CDIR = src
ODIR = obj
IDIR = include

CFLAGS = -lm -Wall -Wextra -I $(IDIR)

SRC = $(wildcard $(CDIR)/*.c)
OBJ = $(patsubst $(CDIR)/%.c, $(ODIR)/%.o, $(SRC))
DEPS = $(wildcard $(IDIR)/*.h)

$(ODIR)/%.o: $(SRC) $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(P): $(OBJ)
	$(CC) $^ -o $(P)
	
all: $(P)

.PHONY: clean

clean:
	rm -rf $(OBJ) $(P)
