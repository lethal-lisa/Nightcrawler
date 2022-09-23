## Makefile for Nightcrawler engine.

## ---------------------------------------------------------------------
## Set phony & default targets, and override the default suffix rules.
## ---------------------------------------------------------------------

.PHONY: all clean
.SUFFIXES:

.DEFAULT_GOAL := all

## ---------------------------------------------------------------------
## Setup project details.
## ---------------------------------------------------------------------

TARGET   := $(shell basename $(CURDIR))
SOURCES  := src/
BUILD    := build/
INCLUDE  := inc/
OBJS     := $(patsubst $(SOURCES)%.c,$(BUILD)%.o,$(wildcard src/*.c))

## ---------------------------------------------------------------------
## Set flags for code generation.
## ---------------------------------------------------------------------

ifdef WINDOWS
ifdef WIN64
	CC   := x86_64-w64-mingw32-gcc
	LD   := x86_64-w64-mingw32-gcc
else
	CC   := i686-w64-mingw32-gcc
	LD   := i686-w64-mingw32-gcc
endif
	TARGET := $(TARGET).exe
else
	CC   := gcc
	LD   := gcc
endif
#OBJCOPY := objcopy

CFLAGS   := -Wall -Os -fno-printf-return-value -fshort-enums
ifdef DEBUG
	CFLAGS += -D_DEBUG -g
endif
ifdef WINDOWS
	CFLAGS += -D_WINDOWS
ifdef WIN64
CFLAGS += -D_WIN64
else
CFLAGS += -D_WIN32
endif
endif
LDFLAGS  := $(CFLAGS)
CFLAGS   += -I$(INCLUDE)

## ---------------------------------------------------------------------
## Compilation rules.
## ---------------------------------------------------------------------

## Compile all targets.
all: $(TARGET)
	-@chmod +x $<

## Link objects.
$(TARGET): $(OBJS)
	-@echo 'Linking objects... ("$^"->"$@")'
	$(LD) $(LDFLAGS) $^ -o $@

## Compile each C file.
$(OBJS): $(BUILD)%.o : $(SOURCES)%.c
	-@mkdir -p $(BUILD)
	-@echo 'Compiling object "$@"... ("$<"->"$@")'
	$(CC) $(CFLAGS) -c $< -o $@

## Remove $(BUILD) dir.
.IGNORE: clean
clean:
	@echo 'Cleaning up intermediary files...'
	@rm -rvf $(BUILD) $(TARGET) $(TARGET).exe
