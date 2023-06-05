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

CFLAGS   := -Wall -Oz -fshort-enums
ifndef CLANG
	# -fno-printf-return-value unsupported on clang.
	CFLAGS += -fno-printf-return-value
endif

ifdef DEBUG
	CFLAGS += -g -D_DEBUG
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

## ---------------------------------------------------------------------
## Compilation rules.
## ---------------------------------------------------------------------

## Generate a 7zip release archive (requires p7zip).
%.7z: default.nst LICENSE README.md $(TARGET)
	-@echo 'Compressing 7zip release archive ("$^"->"$@")'
	$(STRIP) -vs $(TARGET)
	7z a $@ $^
	sha256sum -b $@ > $@.sha256

## Generate a zip release archive (requires infozip).
%.zip: default.nst LICENSE README.md $(TARGET)
	-@echo 'Compressing zip release archive ("$^"->"$@")'
	$(STRIP) -vs $(TARGET)
	zip -v -9 $@ $^
	sha256sum -b $@ > $@.sha256

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
