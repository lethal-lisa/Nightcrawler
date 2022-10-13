## Makefile for Nightcrawler engine.

## ---------------------------------------------------------------------
## Set phony & default targets, and override the default suffix rules.
## ---------------------------------------------------------------------

.PHONY: all clean release
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
	GCCPREFIX := x86_64-w64-mingw32-
else
	GCCPREFIX := i686-w64-mingw32-
endif
	TARGET    := $(TARGET).exe
endif

ifndef CLANG
	CC        := $(GCCPREFIX)gcc
	LD        := $(GCCPREFIX)gcc
	STRIP     := $(GCCPREFIX)strip
else
	CC        := clang
	LD        := clang
	STRIP     := strip
endif

CFLAGS   := -Wall -Os -fshort-enums
ifndef CLANG
	# -fno-printf-return-value unsupported on clang.
	CFLAGS += -fno-printf-return-value
endif

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

## Generate a release archive.
release: all
	$(STRIP) -vs $(TARGET)
	md5sum -b default.nst LICENSE README.md $(TARGET) | tee nightcrawler.md5
	7z a nightcrawler.7z nightcrawler.md5 default.nst LICENSE README.md $(TARGET)

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
