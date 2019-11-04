# vim: set ft=make:
#
# Modular Makefile
#
# Author: Felix Viernickel
#   Date: 2019
#
# This makefile is supposed to compile executables and libraries with
# sane defaults and serve as a basic "just-compile-it" configuration for new
# projects. Keep all code you want to compile under $(SRCDIR),
# includes in $(INCDIR) and libs in $(LIBDIR) and this works like a
# charm.
#
# OOTB you can:
#  make [compile/release]                 # with or without optimization
#  make [debug/asan/tsan]                 # different debug builds
#  make [clean/skel]                      # some miscellaneous stuff
#  make [binary/libstatic/libshared]      # NOTE: set a default one in the "all" target!

# Grab a copy of ctags:
#  make tags
#
# Grab a copy of cheat.h:                 # from https://github.com/Tuplanolla/cheat
#  make test
#
# ..or don't.
#
# {{{ Project
#
# These are the most important settings to get started.
#

# The name of your main file will also be the name of
# the output file. Libraries don't *really* have a main
# file, but you know what I mean.
MAIN=meeus-season

# The default target. It's important to adjust this to
# your needs. Predefined values are:
#     binary / libstatic / libshared
all:
	@$(MAKE) binary

# Adjust the projects directories as needed.
SRCDIR=src
LIBDIR=lib
INCDIR=include
TESTDIR=test

# Output directories.
# WARNING: "make clean" WILL delete these directories.
# Be careful when changing these values. Don't point
# them at important stuff.
BINDIR=bin
OBJDIR=obj

# }}}
# {{{ Compiler Settings
#
# The most important project settings. Adjust this to your needs.
#

LIBS=-lm
INCDIRS=-I. -I$(INCDIR) -I$(SRCDIR)
LIBDIRS=-L. -L$(LIBDIR)

CC=gcc
STD=gnu11
WARN=-Wall -Wextra -Wcoverage-mismatch
CFLAGS=-std=$(STD) $(WARN) $(LIBS) -D_GNU_SOURCE

# {{{ Presets
#
# These are the presets used in the mix-and-match
# settings below.
#
MAINOUT=$(MAIN)
OUTDIR=$(BINDIR)
OUTSTR=bin

OBJFLAGS=
LINKFLAGS=
TESTFLAGS=-Wno-unused-parameter -Wno-unused-function -Wno-implicit-fallthrough

# Who cares about "determinism" anyway?
OPTIMIZEFLAGS=-O2 -flto -ffast-math
DEBUGFLAGS=-g

ASANFLAGS=-fsanitize=address						\
	 	  -fsanitize=undefined						\
	 	  -fsanitize=signed-integer-overflow		\
	 	  -fsanitize=shift							\
	 	  -fsanitize=integer-divide-by-zero			\
	 	  -fsanitize=bounds							\
	 	  -fsanitize=vla-bound						\
	 	  -fsanitize=alignment						\
	 	  -fsanitize=object-size					\
	 	  -fsanitize=null							\
	 	  -fsanitize=float-divide-by-zero			\
	 	  -fsanitize=float-cast-overflow			\

TSANFLAGS=-fsanitize=thread

PROFILE_GEN=-fprofile-generate
PROFILE_USE=-fprofile-correction -fprofile-use -freorder-blocks-and-partition -freorder-functions -freorder-blocks

# }}}
# {{{ File & Directory Structure
#
# Below, we find the file and directory structure of
# the project. It's kind of a shotgun approach, but
# will work pretty reliably as long as dependencies
# are not overly complex.
#

# List of *.c files
SRCDEPS=$(shell find $(SRCDIR) -type f -name "*.c")
# List if *.o files (including main)
ALLDEPS=$(SRCDEPS:$(SRCDIR)%.c=$(OBJDIR)%.o)
# List if *.o files (without main)
LIBDEPS=$(ALLDEPS:$(OBJDIR)/$(MAIN)%=)

# Source directory structure
SRCSKEL=$(shell find $(SRCDIR) -type d)
# Object directory structure
OBJSKEL=$(SRCSKEL:$(SRCDIR)%=$(OBJDIR)%)

# List of test *.c files
TESTSRC=$(shell find $(TESTDIR) -type f -name "*.c")
# List of test-suites
TESTBIN=$(TESTSRC:%.c=$(BINDIR)/%)

# Test source directory structure
TESTSRCSKEL=$(shell find $(TESTDIR) -type d)
# Test object directory structure
TESTBINSKEL=$(TESTSRCSKEL:%=$(BINDIR)/%)

# }}}
# {{{ Mix & Match
#
# Apply Settings depending on environment.
#
# In order to "conveniently" mix and match our
# settings, we call make recursively from our
# targets, passing in the appropriate environment
# variables defined below.
#

BINDEPS=$(ALLDEPS)

ifeq ($(DEBUG), 1)
	CFLAGS+=$(DEBUGFLAGS)
endif

ifeq ($(RELEASE), 1)
	CFLAGS+=$(OPTIMIZEFLAGS)
endif

ifeq ($(ASAN), 1)
	CFLAGS+=$(ASANFLAGS)
endif

ifeq ($(TSAN), 1)
	CFLAGS+=$(TSANFLAGS)
endif

ifeq ($(LIBSHARED), 1)
	MAINOUT=$(MAIN).so
	BINDEPS=$(LIBDEPS)
	OUTDIR=$(LIBDIR)
	OUTSTR=lib
	OBJFLAGS=-fPIC
	LINKFLAGS=-shared
endif

ifeq ($(LIBSTATIC), 1)
	MAINOUT=$(MAIN).a
	BINDEPS=$(LIBDEPS)
	OUTDIR=$(LIBDIR)
	OUTSTR=lib
endif

# }}}
# }}}
# {{{ Targets
#
# There should be no need to modify these, but you might
# want to add some of your own.

# {{{ Utility Targets
#
#
# These are catch-all targets for compiling objects, linking executables/libraries, and
# test-suites. Object files are written to the $(OBKDIR) subtree, usually obj/, and executables
# and libraries are output to $(BINDIR), usually bin/.
#
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@printf "compiling [obj]: $(@:$(OBJDIR)/%=%)\n"
	@$(CC) $(INCDIRS) $(LIBDIRS) -o $@ $< -c $(CFLAGS) $(OBJFLAGS)

$(BINDIR)/$(MAINOUT): $(BINDEPS)
	@printf "compiling [$(OUTSTR)]: $(MAINOUT)\n"
	@$(CC) $(INCDIRS) $(LIBDIRS) -o $(BINDIR)/$(MAINOUT) $(BINDEPS) $(CFLAGS) $(LINKFLAGS)

$(BINDIR)/$(TESTDIR)/%: $(TESTDIR)/%.c $(LIBDEPS)
	@printf "compiling [bin]: $(@:$(TESTDIR)/%=%)\n"
	@$(CC) $(INCDIRS) $(LIBDIRS) -o $@ $< $(LIBDEPS) $(CFLAGS) $(TESTFLAGS)

.PHONY: compile
compile: skel $(BINDIR)/$(MAINOUT)
	@printf "done\n"

.PHONY: skel
skel:
	@printf "creating directory layouts\n"
	@mkdir -p $(OBJSKEL) $(TESTBINSKEL) $(BINDIR) $(LIBDIR)

#
# }}}
# {{{ Convenience Targets
#
# Actually define the advertised targets.
#
# Most of these are just shorthand for FOO=1 BAR=1 make.
#
.PHONY: binary
binary: compile
	@printf "making [binary]\n"

.PHONY: libshared
libshared:
	@printf "making [libshared]\n"
	@LIBSHARED=1 $(MAKE) compile

.PHONY: libstatic
libstatic:
	@printf "making [libstatic]\n"
	@LIBSTATIC=1 $(MAKE) compile

.PHONY: debug
debug:
	@printf "setting [debug]\n"
	@DEBUG=1 $(MAKE) all

.PHONY: release
release:
	@printf "setting [release]\n"
	@RELEASE=1 $(MAKE) all

.PHONY: asan
asan:
	@printf "setting [asan]\n"
	@DEBUG=1 ASAN=1 $(MAKE) all

.PHONY: tsan
tsan:
	@printf "setting [tsan]\n"
	@DEBUG=1 TSAN=1 $(MAKE) all

.PHONY: test
test: skel $(TESTBIN)
	@printf "running [all] tests\n"
	@for t in $(TESTBIN); do echo "testing [bin]: $$t"; "$$t"; done

.PHONY: tags
tags:
	@printf "creating [tags]\n"
	@ctags -R

.PHONY: clean
clean:
	@printf "cleaning\n"
	rm -fr $(OBJDIR)/*
	rm -fr $(BINDIR)/*

# }}}
# }}}
