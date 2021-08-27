
OUTFILE = xml.a

ROOTDIR = ../../..

ifndef CONFIGURATION
CONFIGURATION = Debug
endif

ifeq "$(CONFIGURATION)" "Debug"
LIBDIR = $(ROOTDIR)/lib/Debug
endif

ifeq "$(CONFIGURATION)" "Release"
LIBDIR = $(ROOTDIR)/lib/Release
endif

OUTDIR = $(LIBDIR)
OUT = $(OUTDIR)/$(OUTFILE)

MAKEFLAGS += --warn-undefined-variables
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:
.SUFFIXES:

SOURCES := $(wildcard *.cpp)

OBJDIR = $(ROOTDIR)/obj
OBJS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

LIBRARYDIR = library
INCLUDEDIR = -I. \
	-I$(ROOTDIR)/$(LIBRARYDIR)

CPPFLAGS = -std=c++14
CFLAGS = -Wall -g -c
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) $(INCLUDEDIR)

CC = g++

COMPILE.cpp = $(CC) $(ALL_CFLAGS)

.PHONY: all
all: $(OUT)

$(OUT): $(OBJS)
	ar -rs $@ $(OBJS)

DEPDIR = $(ROOTDIR)/dep
DEPFILEIN = $(OBJDIR)/$(*F)
DEPFILEOUT = $(DEPDIR)/$(*F)
DEPS = $(SOURCES:%.cpp=$(DEPDIR)/%.P)

$(OBJDIR)/%.o : %.cpp
	$(COMPILE.cpp) -MD -o $@ $<
	@cp $(DEPFILEIN).d $(DEPFILEOUT).P; \
        sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
            -e '/^$$/ d' -e 's/$$/ :/' < $(DEPFILEIN).d >> $(DEPFILEOUT).P; \
        rm -f $(DEPFILEIN).d

-include $(DEPS)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(DEPS): | $(DEPDIR)

$(DEPDIR):
	mkdir -p $(DEPDIR)

$(OUT): | $(OUTDIR)

$(OUTDIR):
	mkdir -p $(OUTDIR)

.PHONY: clean
clean:
	rm -f $(DEPS) $(OBJS) $(OUT)

#.PHONY: install
#install:

.PHONY: run
run:
	./$(OUT)
