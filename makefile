## Makefile for Nightcrawler engine.

## ---------------------------------------------------------------------
## Set phony & default targets, and override the default suffix rules.
## ---------------------------------------------------------------------

.PHONY: all clean debug
.SUFFIXES:

.DEFAULT_GOAL := all

## ---------------------------------------------------------------------
## Setup project details.
## ---------------------------------------------------------------------

TARGET   := $(shell basename $(CURDIR))
SOURCES  := src/
BUILD    := build/
#INCLUDE  := inc/
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

## Generate a 7zip release archive (requires p7zip).
%.7z: default.nst LICENSE README.md $(TARGET)
	-@echo 'Compressing 7zip release archive ("$^"->"$@")'
	$(STRIP) -vs $(TARGET)
	7z a $@ $^

## Generate a zip release archive (requires infozip).
%.zip: default.nst LICENSE README.md $(TARGET)
	-@echo 'Compressing zip release archive ("$^"->"$@")'
	$(STRIP) -vs $(TARGET)
	zip -v -9 $@ $^

## Use debug build.
debug: CFLAGS += -D_DEBUG -g
debug: all

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
