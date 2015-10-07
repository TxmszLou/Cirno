CC=clang++
CFLAGS=-std=c++14 -O3
SRC=main.cpp
INCLUDES = -I .
INSTALL   ?= install
MKDIR     ?= $(INSTALL) -d
BINDIR    ?= $(PREFIX)/bin
DESTDIR   ?=

all: cirno

rebuild: clean | all

cirno: $(SRC)
	$(CC) ${INCLUDES} -o cirno $^ ${CFLAGS}

.PHONY: clean all rebuild

clean:
	rm -f cirno

install:
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) cirno$(EXE) $(DESTDIR)$(BINDIR)/

