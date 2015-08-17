###############################################################################
##
## GNU Makefile for Arjuna
##
## Programmer:
## 	Ilham Imaduddin
##	Ahmad Shalahuddin
##	Piquitha Della Audyna
##
## Gadjah Mada University
## 2015
##
###############################################################################

# Programs
CC = g++
RM = rm

# Files and Directories
SRCDIR = src
BUILDDIR = build
INCDIR = include
LIBDIR = lib
BINDIR = bin
TARGET = arjuna

SRCEXT = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Compiler Options
STD = -std=c++11
OPT = 1
LFLAGS = -lasound -lpthread -lwiringPi -lwiringPiDev -L$(LIBDIR) -lmidifile
DFLAGS = -D__LINUX_ALSA__ -D__LITTLE_ENDIAN__
INC = -I $(INCDIR)
CFLAGS = -Wall $(STD) -O$(OPT)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	@mkdir -p $(BINDIR)
	@echo "$(CC) -o $(BINDIR)/$(TARGET) $^ $(LFLAGS)"; $(CC) $^ -o $(BINDIR)/$(TARGET) $(LFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) $(DFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(DFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning..."
	@echo "$(RM) -r $(BUILDDIR)/$(TARGET)"; $(RM) -r $(BUILDDIR)/$(TARGET)

.PHONY: clean