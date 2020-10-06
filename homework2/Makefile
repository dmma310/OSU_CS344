# Comprehensive Makefile
<<<<<<< HEAD
# This will automatically compile any .cpp files in the current directory.
=======
# This will automatically compile any .c files in the current directory.
>>>>>>> 6b9448e308d92dbbe12e3f20c1ce04ac197df1e3
# Some of the syntax was adapted from:
# https://gist.github.com/Wenchy/64db1636845a3da0c4c7
# http://nuclear.mutantstargoat.com/articles/make/

<<<<<<< HEAD
CC = g++
CFLAGS := -std=c++11
=======
CC = gcc
CFLAGS = --std=gnu99
SRCEXT = c
>>>>>>> 6b9448e308d92dbbe12e3f20c1ce04ac197df1e3

BINDIR = .
exe_file = $(BINDIR)/$(shell basename "${PWD}")

# Handle debug case
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -g -Wall
else
	CFLAGS += -DNDEBUG -O3
endif

SRCDIR = .
BUILDDIR = build
<<<<<<< HEAD
SRCEXT = cpp
=======
>>>>>>> 6b9448e308d92dbbe12e3f20c1ce04ac197df1e3
SOURCES = $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEP = $(OBJECTS:.o=.d)

all: $(exe_file)

$(exe_file): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) -o $(exe_file) $^ $(LIB) $(LDFLAGS)

$(BUILDDIR)/%.d: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@$(CC) $(INC) $< -MM -MT $(@:.d=.o) >$@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(exe_file)

-include $(DEP)

