CC=gcc
AR=ar rcs
RM=rm
CFLAGS=-Wall
LIB=jf.a
OBJ=jf.o
PREFIX=/usr/local

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $@ $?

clean:
	$(RM) $(OBJ) $(LIB)

install:
	cp jf.h $(PREFIX)/include
	cp $(LIB) $(PREFIX)/lib
